//
// Created by hiccup on 2023/11/9.
//

#include "Mem2Reg.h"

#include "../llvm-ir/Value.h"
#include "../llvm-ir/User.h"
#include "../llvm-ir/BasicBlock.h"
#include "../llvm-ir/instruction/Instruction.h"
#include "../llvm-ir/instruction/AllocaInst.h"
#include "../llvm-ir/Module.h"
#include "../llvm-ir/IrBuilder.h"

#include <set>
#include <iostream>

/// algorithm from https://buaa-se-compiling.github.io/miniSysY-tutorial/challenge/mem2reg/help.html
/// @defAndUse  ->  @insertPhi  ->  @rename

Instruction *globalInst;

void Mem2Reg::run(Module *module) {
	for (auto func : module->functionList) {
		for (auto basicBlock : func->basicList) {

			auto *instList = &(basicBlock->instructionList);
			for (auto it = instList->begin(); it!=instList->end();) {
				auto inst = *it;


				if (inst->name=="%t_len_0")
					int a=24;



				if (inst->instType==InstType::ALLOCA &&
					dynamic_cast<AllocaInst *>(inst)->retType==IntegerType::INT32) {

					buildDefUse(inst);
					insertPhiInst(inst);

					rename(func->basicList.front());
					it = instList->erase(it);

				} else {
					++it;
				}
//				cout << irBuilder.getModule()->toString() << endl;
			}

//			auto instList = basicBlock->instructionList;
//			for (auto inst : instList) {
//				if (inst->instType==InstType::ALLOCA &&
//					dynamic_cast<AllocaInst *>(inst)->retType==IntegerType::INT32) {
//
//					globalInst = inst;
//
//					buildDefUse(inst);
//					insertPhiInst(inst);
//
//					rename(func->basicList.front());
//
//				}
//			}

		}
	}
	1;
}

void Mem2Reg::buildDefUse(Instruction *inst) {
	defBbs = new std::set<BasicBlock *>();
	useBbs = new std::set<BasicBlock *>();
	defInsts = new std::set<Instruction *>();
	useInsts = new std::set<Instruction *>();

	for (auto use : inst->useList) {

		Instruction *userInst = ((Instruction *)use->user);
		if (userInst->instType==InstType::STORE) {
			defBbs->insert(userInst->parent);
			defInsts->insert(userInst);
		}
		if (userInst->instType==InstType::LOAD) {
			useBbs->insert(userInst->parent);
			useInsts->insert(userInst);
		}
	}

}

void Mem2Reg::insertPhiInst(Instruction *inst) {





	/// it was me a silly b, i don't remember why it can't pass the test at that time
	/// maybe something else trigger this error
	/// there should be @no phi

	//	if (defBbs->size()==1)
	//		return;



	auto *F = new std::set<BasicBlock *>();
	auto *W = new std::set<BasicBlock *>();

	W->insert(defBbs->begin(), defBbs->end());

	while (!W->empty()) {
		BasicBlock *X = *(W->begin());
		W->erase(X);
		for (BasicBlock *Y : (*X->DF)) {
			if (!F->count(Y)) {

				auto *phi = (Instruction *)irBuilder.buildPhiInst(Y->preBbs, Y);
				useInsts->insert(phi);
				defInsts->insert(phi);

				F->insert(Y);
				if (!defBbs->count(Y)) {
					W->insert(Y);
				}
			}
		}
	}

}

/*
 * 遇到局部 int 类型变量对应的 load 指令，将所有其他指令中对该指令的使用替换为对该变量到达定义的使用，删除 load 指令；
 * 遇到局部 int 类型变量对应的 store 指令，你需要更新该变量的到达定义，删除 store 指令.
 * 遍历完成一个基本块中的所有指令后，你需要维护该基本块的所有后继基本块中的 phi 指令，将对应来自此基本块的值设为对应变量的到达定义。
 *
 */


/// use @stack to maintain @reachDef
void Mem2Reg::rename(BasicBlock *entry) {

	int cnt = 0;
	auto *instList = &(entry->instructionList);

	for (auto it = instList->begin(); it!=instList->end();) {
		auto inst = (Instruction *)(*it);
		if (inst->instType==InstType::LOAD && useInsts->count(inst)) {

			inst->replaceOld2New(!reach->empty() ? reach->top() : new ConstantInt(0,0));
			it = instList->erase(it);
		} else if (inst->instType==InstType::STORE && defInsts->count(inst)) {
			/// store's @content
			reach->push(inst->getOperand(0));
			++cnt;
			it = instList->erase(it);
		}
//		else if (inst->instType==InstType::ALLOCA && inst == globalInst) {
//			it = instList->erase(it);
//		}
		else {

			if (inst->instType==InstType::PHI && defInsts->count(inst)) {
				reach->push(inst);
				++cnt;
			}

			++it;
		}
	}


	// find all the phiInst in the successors
	for (auto i : *entry->sucBbs) {
		for (auto j : i->instructionList) {
			if (j->instType!=InstType::PHI)
				break;
			if (useInsts->count(j)) {
				((PhiInst *)j)->addValueFromBb(!reach->empty() ? reach->top() :new ConstantInt(0,0) , entry);
			}
		}
	}



	/// @childBbs in the tree, @not successors in the CFG
	for (BasicBlock *next : (*entry->childBbs)) {
		rename(next);
	}

	for (int i = 0; i < cnt; ++i) {
		reach->pop();
	}

}

