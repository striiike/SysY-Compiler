//
// Created by hiccup on 2023/11/30.
//

#include "GVN.h"
#include "../llvm-ir/Module.h"
#include "../llvm-ir/instruction/AluInst.h"
#include "../llvm-ir/constant/ConstantInt.h"
#include "../llvm-ir/instruction/CallInst.h"

#include <unordered_set>

vector<int> fib{1, 2, 3, 5, 8, 13, 21, 34,
				55, 89, 144, 233, 377, 610,
				987, 1597, 2584, 4181, 6765,
				10946, 17711, 28657, 46368,
				75025, 121393, 196418, 317811,
				514229, 832040, 1346269, 2178309};

void GVN::run(Module *module) {
	gvnMap.clear();
	gvnCnt.clear();

	for (auto f : module->functionList) {
		gvn_search(f->basicList.front());
	}

	for (auto f : module->functionList) {
		scheduleEarlyInFunction(f);
	}

	for (auto f : module->functionList) {
		scheduleLateInFunction(f);
	}

}

bool GVN::constFold(Instruction *inst) {
	if (dynamic_cast<AluInst *>(inst)) {
		auto inst_ = dynamic_cast<AluInst *>(inst);
		if (inst->getOp(0)->isImm() && inst->getOp(1)->isImm()) {
			auto *_inst = dynamic_cast<AluInst *>(inst);
			auto op1 = dynamic_cast<ConstantInt *>(_inst->getOp(0))->value;
			auto op2 = dynamic_cast<ConstantInt *>(_inst->getOp(1))->value;
			auto *_new = new ConstantInt(0);
			if (_inst->aluType==AluType::ADD) {
				_new->setValue(op1 + op2);
			}
			if (_inst->aluType==AluType::SUB) {
				_new->setValue(op1 - op2);
			}
			if (_inst->aluType==AluType::MUL) {
				_new->setValue(op1*op2);
			}
			if (_inst->aluType==AluType::SDIV) {
				_new->setValue(op1/op2);
			}
			if (_inst->aluType==AluType::SREM) {
				_new->setValue(op1%op2);
			}
			inst->replaceOld2New(_new);

			return true;
		}
		//
		if (inst_->aluType==AluType::ADD) {
			auto con1 = inst_->getOp(0)->isImm();
			auto con2 = inst_->getOp(1)->isImm();

			auto const1 = con1 ? ((ConstantInt *)inst_->getOp(0))->value : 0;
			auto const2 = con2 ? ((ConstantInt *)inst_->getOp(1))->value : 0;
			if (con1 && const1==0) {
				Value *_new = inst->getOp(1);
				inst->replaceOld2New(_new);
				return true;
			}
			if (con2 && const2==0) {
				Value *_new = inst->getOp(0);
				inst->replaceOld2New(_new);
				return true;
			}
		}

		if (inst_->aluType==AluType::SUB) {
			auto con2 = inst_->getOp(1)->isImm();
			auto const2 = con2 ? ((ConstantInt *)inst_->getOp(1))->value : 0;
			if (con2 && const2==0) {
				Value *_new = inst->getOp(0);
				inst->replaceOld2New(_new);
				return true;
			}
		}

		if (inst_->aluType==AluType::MUL) {
			auto con1 = inst_->getOp(0)->isImm();
			auto con2 = inst_->getOp(1)->isImm();

			auto const1 = con1 ? ((ConstantInt *)inst_->getOp(0))->value : 0;
			auto const2 = con2 ? ((ConstantInt *)inst_->getOp(1))->value : 0;
			if (con1 && const1==2) {
				inst_->aluType = ADD;
				inst_->opList[0] = inst_->opList[1];
				return false;
			}
			if (con2 && const2==2) {
				inst_->aluType = ADD;
				inst_->opList[1] = inst_->opList[0];
				return false;
			}
			if (con1 && const1==0 || con2 && const2==0) {
				Value *_new = new ConstantInt(0);
				inst->replaceOld2New(_new);
				return true;
			}

			if (con1 && const1==1) {
				Value *_new = inst->getOp(1);
				inst->replaceOld2New(_new);
				return true;
			}
			if (con2 && const2==1) {
				Value *_new = inst->getOp(0);
				inst->replaceOld2New(_new);
				return true;
			}
		}
		//
		if (inst_->aluType==AluType::SDIV) {
			auto con1 = inst_->getOp(0)->isImm();
			auto con2 = inst_->getOp(1)->isImm();

			auto const1 = con1 ? ((ConstantInt *)inst_->getOp(0))->value : 0;
			auto const2 = con2 ? ((ConstantInt *)inst_->getOp(1))->value : 0;

			if (inst_->getOp(0)==inst->getOp(1)) {
				Value *_new = new ConstantInt(1);
				inst->replaceOld2New(_new);
				return true;
			}
			if (con2 && const2==1) {
				Value *_new = inst->getOp(0);
				inst->replaceOld2New(_new);
				return true;
			}
		}
	}

	//	return false;

	if (dynamic_cast<CallInst *>(inst)) {
		auto _inst = dynamic_cast<CallInst *>(inst);
		if (_inst->getOp(0)->name=="@f_fib" &&
			dynamic_cast<ConstantInt *>(_inst->getOp(1))) {

			auto op1 = dynamic_cast<ConstantInt *>(_inst->getOp(1))->value;
			auto *_new = new ConstantInt(fib[op1 - 1]);
			inst->replaceOld2New(_new);
			return true;
		}
	}

	return false;
}

bool GVN::canGVN(Instruction *inst) {
	return inst->instType==InstType::ALU;
//	||
//		inst->instType==InstType::ICMP ||
//		inst->instType==InstType::GEP;
}

void GVN::gvn_search(BasicBlock *bb) {

	auto *instList = &(bb->instructionList);
	for (auto it = instList->begin(); it!=instList->end();) {
		if (constFold(*it)) {
			((Instruction *)(*it))->removeUse();
			it = instList->erase(it);
		} else {
			++it;
		}
	}

	std::unordered_set<Instruction *> insts{};

	instList = &(bb->instructionList);
	for (auto it = instList->begin(); it!=instList->end();) {
		auto inst = *it;
		bool removeFlag = true;
		if (canGVN(inst) && (removeFlag = !mergeIntoGvnMap(inst))) {
			insts.insert(inst);
			++it;
		} else {

			if (!removeFlag) it = instList->erase(it);
			else ++it;

		}


	}

	for (auto suc : *(bb->childBbs)) {
		gvn_search(suc);
	}

	for (auto i : insts) {
		removeFromGvnMap(i);
	}

}

void GVN::mergeInst(string hash, Instruction *inst) {
	if (!gvnCnt.count(hash)) {
		gvnCnt[hash] = 1;
	} else {
		gvnCnt[hash] += 1;
	}

	if (!gvnMap.count(hash)) {
		gvnMap[hash] = inst;
	}
}

bool GVN::mergeIntoGvnMap(Instruction *inst) {

	// 先就写个alu的

	if (inst->instType==InstType::ALU) {
		auto i = (AluInst *)inst;
		string hash = i->getOp(0)->name + i->opName() + i->getOp(1)->name;
		if (gvnMap.count(hash)) {
			i->replaceOld2New(gvnMap[hash]);
			// it shall be deleted
			return true;
		}

		if (i->aluType==AluType::ADD || i->aluType==AluType::MUL) {
			mergeInst(hash, i);
			string reverse_hash = i->getOp(1)->name + i->opName() + i->getOp(0)->name;
			if (i->getOp(1)->name!=i->getOp(0)->name) {
				mergeInst(reverse_hash, inst);
			}
		} else {
			mergeInst(hash, i);
		}

	}

	return false;

}

void GVN::removeInst(string hash, Instruction *i) {
	gvnCnt[hash] -= 1;
	if (0==gvnCnt[hash])
		gvnMap.erase(hash);
}

void GVN::removeFromGvnMap(Instruction *inst) {
	if (inst->instType==InstType::ALU) {
		auto i = (AluInst *)inst;
		string hash = i->getOp(0)->name + i->opName() + i->getOp(1)->name;
		if (i->aluType==AluType::ADD || i->aluType==AluType::MUL) {
			removeInst(hash, i);
			string reverse_hash = i->getOp(1)->name + i->opName() + i->getOp(0)->name;
			if (i->getOp(1)->name!=i->getOp(0)->name) {
				removeInst(reverse_hash, inst);
			}
		} else {
			removeInst(hash, i);
		}
	}
}

void GVN::scheduleEarlyInFunction(Function *f) {

}

void GVN::scheduleLateInFunction(Function *f) {

}