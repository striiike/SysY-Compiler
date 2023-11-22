//
// Created by hiccup on 2023/11/22.
//
#include <iostream>
#include <set>
#include <algorithm>
#include "RemovePhi.h"
#include "../llvm-ir/instruction/PhiInst.h"
#include "../llvm-ir/instruction/ParallelCopyInst.h"
#include "../llvm-ir/IrBuilder.h"

void RemovePhi::run(Module *module) {
	for (auto f : module->functionList) {

		/// making non-conventional SSA form conventional
		/// by replacing with parallel copy
		parallelCopy(f);

//		cout << module->toString() << endl;

		/// change parallel copy to mips-fit version
		sequentialMove(f);

	}
}

void insertParallelCopy(ParallelCopyInst *pc, BasicBlock *to) {
	/// it is under guarantee that last instruction is branch
	auto *list = &(to->instructionList);
	for (auto rit = list->rbegin(); rit!=list->rend(); ++rit) {
		if ((*rit)->instType==InstType::BRANCH) {
			list->insert(--(rit.base()), pc);
			break;
		}
	}
}

BasicBlock *insertBasicBlock(BasicBlock *pre, BasicBlock *suc) {
	auto *mid = irBuilder.buildBb();
	auto *f = pre->parent;
	mid->parent = f;

	auto *list = &(f->basicList);

	for (auto it = list->begin(); it!=list->end(); ++it) {
		if (*it==pre) {
			list->insert(++it, mid);
			break;
		}

	}

	auto it = std::find(pre->sucBbs->begin(), pre->sucBbs->end(), suc);
	*it = mid;

	auto it1 = std::find(suc->preBbs->begin(), suc->preBbs->end(), pre);
	*it1 = mid;

	mid->preBbs = new vector<BasicBlock *>;
	mid->sucBbs = new vector<BasicBlock *>;
	mid->preBbs->push_back(pre);
	mid->sucBbs->push_back(suc);

	return mid;
}

void RemovePhi::parallelCopy(Function *f) {

	/// only need to consider the blocks with phiInstruction
	for (auto curBb : f->basicList) {

//		if (curBb->name == "b_3") {
//			int a =234;
//		}


		if (InstType::PHI!=curBb->instructionList.front()->instType)
			continue;

		// in order to fill pc later
		vector<ParallelCopyInst *> pcList{};

		/// in order to avoid changing in iteration
		auto list = *curBb->preBbs;
		for (auto preBb : list) {
			auto *pc = new ParallelCopyInst();
			pcList.push_back(pc);

			if (preBb->sucBbs->size() > 1) {
				/// replace curBb -> preBb by {curBb -> newBb, newBb -> preBb}
				auto *mid = insertBasicBlock(preBb, curBb);

				/*
				 *  what we need to do here: (not necessarily in that order)
				 *  	; update cfg info (done in insert bb)
				 *  	; change branch instruction
				 *  	; insert parallel copy
				 */

				// last instruction is branch, which is under guarantee by cfg;
				auto branch = (BrInst *)preBb->instructionList.back();
				if (branch->jump) {
					branch->operandList[0] = mid;
				} else {
					if (branch->operandList[1]==curBb) {
						branch->operandList[1] = mid;
					}
					if (branch->operandList[2]==curBb) {
						branch->operandList[2] = mid;
					}
				}
				irBuilder.curBb = mid;
				mid->instructionList.push_back(pc);
				irBuilder.buildBrInst(curBb);

			} else {
				/*
				 *  only need to insert phi into the pre BasicBlock
				 */
				insertParallelCopy(pc, preBb);

			};
		}


		// because phi's preBb is exactly the same as curBb's preBb
		// it is under guarantee by the definition of phi instruction
		// so parallel copy inst options' quantity is equal to phi instruction options' quantity;
		for (auto inst : curBb->instructionList) {
			if (inst->instType!=InstType::PHI)
				break;

			for (int i = 0; i < pcList.size(); ++i) {
				pcList[i]->copyPair(inst, inst->operandList[i]);
			}

		}

		// kill all the phi Inst;
		curBb->instructionList.remove_if([](Instruction *i) { return i->instType==InstType::PHI; });
	}

}

bool checkPC(vector<pair<Value *, Value *>> *pcList) {
	for (auto i : *pcList) {
		if (i.first->name!=i.second->name)
			return false;
	}
	return true;
}

void RemovePhi::sequentialMove(Function *f) {

	/*
	 * here is to make parallel copy into sequential assignment
	 */

	/*
	 * I think it's better to make all the parallel copy into a move list,
	 * 	then replace the parallel copy with it
	 */
	for (auto curBb : f->basicList) {

		vector<pair<Value *, Value *>> pcList{};
		vector<MoveInst *> seq{};

		for (auto inst : curBb->instructionList) {
			if (inst->instType==InstType::PC) {
				auto *pc = (ParallelCopyInst *)inst;
				auto tmpList = pc->pairList();
				pcList.insert(pcList.end(), tmpList.begin(), tmpList.end());
			}
		}

		/*
		 * implement exactly based on the algorithm
		 */


		/// @caution use & carefully

		while (!checkPC(&pcList)) {
			int flag = true;
			pair<Value *, Value *> *edge = nullptr;
			for (int i = 0; i < pcList.size(); ++i) {
				edge = &pcList[i];

				flag = true;
				for (auto &j : pcList) {
					if (edge->first==j.second) {
						flag = false;
					}
				}
				if (flag)
					break;
			}

			if (flag) {
				seq.push_back(new MoveInst(edge->first, edge->second));
				for (auto it = pcList.begin(); it != pcList.end(); ++it) {
					if (*it == *edge) {
						pcList.erase(it);
						break;
					}
				}
			} else {
				auto *val = new Value(IntegerType::INT32, irBuilder.genLocalVarName("moveTemp"));
				seq.push_back(new MoveInst(val, edge->second));
				edge->second = val;
			}
		}

		// kill all the parallel copy
		curBb->instructionList.remove_if([](Instruction *i) { return i->instType==InstType::PC; });

		// insert all the move
		auto ptr = curBb->instructionList.end();
		curBb->instructionList.insert(--ptr, seq.begin(), seq.end());
	}

}