//
// Created by hiccup on 2023/11/14.
//

#include "MipsParser.h"
#include "../llvm-ir/Module.h"
#include "../llvm-ir/constant/ConstantInt.h"
#include "../llvm-ir/constant/ConstantArray.h"
#include "../llvm-ir/constant/ConstantString.h"
#include "../llvm-ir/instruction/AllocaInst.h"
#include "../llvm-ir/instruction/ZextInst.h"
#include "../llvm-ir/instruction/AluInst.h"
#include "../llvm-ir/instruction/BrInst.h"
#include "../llvm-ir/instruction/GEPInst.h"
#include "../llvm-ir/instruction/CallInst.h"
#include "../llvm-ir/instruction/LoadInst.h"
#include "../llvm-ir/instruction/StoreInst.h"
#include "../llvm-ir/instruction/ReturnInst.h"
#include "../llvm-ir/instruction/IcmpInst.h"
#include "../llvm-ir/instruction/ParallelCopyInst.h"

auto mipsBuilder = new MipsBuilder();

void MipsParser::parseModule() {
	for (auto i : module->globalList) {
		parseGlobalVar(i);
	}
	for (auto i : module->functionList) {
		parseFunction(i);
	}
}

void MipsParser::parseGlobalVar(GlobalVariable *glo) const {
	auto *mipsGlo = new MipsGlobalVar();
	mipsGlo->name = glo->name.substr(1);
	mipsGlo->label = new MipsLabel(mipsGlo->name);

	(*global2Operand)[glo] = mipsGlo->label;

	if (glo->init->isArr()) {
		auto item = ((ConstantArray *)glo->init);
		mipsGlo->arr = item->array;
	}
	if (glo->init->isInt()) {
		auto item = ((ConstantInt *)glo->init);
		mipsGlo->arr.push_back(item->value);
	}
	if (glo->init->isStr()) {
		auto item = ((ConstantString *)glo->init);
		mipsGlo->stringLiteral = item->mipsString();
	}
	mipsModule->globals.push_back(mipsGlo);
}

void MipsParser::parseFunction(Function *func) {
	curMipsFunction = new MipsFunction();
	mipsModule->functions.push_back(curMipsFunction);
	for (auto i : func->basicList) {
		curMipsBlock = new MipsBlock();
		curMipsFunction->blockList.push_back(curMipsBlock);
		for (auto j : i->instructionList) {
			parseInstruction(j, curMipsBlock);
		}
	}
}

void MipsParser::parseInstruction(Instruction *inst, MipsBlock *block) {
	block->instructionList.push_back(new MipsComment(inst->toString()));
	switch (inst->instType) {
	case InstType::ALLOCA: {
		parseAllocaInst((AllocaInst *)inst);
		break;
	}
	case InstType::ALU: {
		parseAluInst((AluInst *)inst);
		break;
	}
	case InstType::BRANCH: {
		parseBrInst((BrInst *)inst);
		break;
	}
	case InstType::CALL: {
		parseCallInst((CallInst *)inst);
		break;
	}
	case InstType::GEP: {
		parseGEPInst((GEPInst *)inst);
		break;
	}
	case InstType::ICMP: {
		parseIcmpInst((IcmpInst *)inst);
		break;
	}
	case InstType::LOAD: {
		parseLoadInst((LoadInst *)inst);
		break;
	}
	case InstType::RETURN: {
		parseReturnInst((ReturnInst *)inst);
		break;
	}
	case InstType::STORE: {
		parseStoreInst((StoreInst *)inst);
		break;
	}
	case InstType::MOVE: {
		parseMoveInst((MoveInst *)inst);
		break;
	}
	case InstType::ZEXT: {
		parseZextInst((ZextInst *)inst);
		break;
	}
	default: break;
	}
}

/*
 *  most of the compilers I observed have this kind of method,
 *  it is really important in every aspect.
 *  It is an abstract way of changing irValue into MipsOperand.
 *
 */
MipsOperand *MipsParser::parseOperand(Value *val, bool canImm) {
	if (value2Operand->count(val)) {
		auto op = (*value2Operand)[val];
		if (!canImm && dynamic_cast<MipsImm *> (op)) {
			auto *tmp = new MipsVrReg();
			auto *inst = new MipsLiInst(tmp, op);
			curMipsBlock->addInst(inst);
			return tmp;
		}
		return op;
	}

	if (dynamic_cast<Value *>(val)) {
		return parseGlobalOp(dynamic_cast<GlobalVariable *>(val), canImm);
	} else if (dynamic_cast<Argument *>(val)) {
		return parseArgumentOp(dynamic_cast<Argument *>(val), canImm);
	} else if (dynamic_cast<ConstantInt *>(val)) {
		return parseConstOp(dynamic_cast<ConstantInt *>(val), canImm);
	} else {
		auto *vr = new MipsVrReg();
		return vr;
	}
}

MipsOperand *MipsParser::parseGlobalOp(GlobalVariable *val, bool canImm) {
	auto *dst = new MipsVrReg();
	auto *inst = new MipsLiInst(dst, (*global2Operand)[val]);
	curMipsBlock->addInst(inst);
	return dst;
}

MipsOperand *MipsParser::parseArgumentOp(Argument *val, bool canImm) {
	int rk = val->rank;
	auto *dst = new MipsVrReg();
	if (rk < 4) {
		auto *inst = new MipsLiInst(dst, (rk==0) ? $a0 : (rk==1) ? $a1 : (rk==2) ? $a2 : (rk==3) ? $a3 : nullptr);
		curMipsBlock->addInst(inst);
	} else {

	}
	return dst;
}

MipsOperand *MipsParser::parseConstOp(ConstantInt *val, bool canImm) {
	return new MipsImm(val->value);
}

void MipsParser::parseAllocaInst(AllocaInst *inst) {
	int len;
	if (inst->retType==IntegerType::INT32) {
		len = 4;
		mipsBuilder->stackOffset -= 4;
	} else {
		len = ((ArrayType *)inst->retType)->getNum();
		mipsBuilder->stackOffset -= 4*len;
	}

	mipsBuilder->buildBinInst(BinType::M_ADDU, $t0, $sp, new MipsImm(mipsBuilder->stackOffset));
	mipsBuilder->stackOffset -= 4;

	mipsBuilder->buildStoreInst($t0, new MipsImm(mipsBuilder->stackOffset), $sp);
	(*mipsBuilder->value2Offset)[inst] = mipsBuilder->stackOffset;
}

void MipsParser::parseAluInst(AluInst *inst) {
	auto op1 = inst->getOp(0), op2 = inst->getOp(1);
	bool imm1 = dynamic_cast<ConstantInt *> (op1);
	bool imm2 = dynamic_cast<ConstantInt *> (op2);

	auto dst = parseOperand(inst, false);
	MipsInst *mipsInst;
	if (inst->aluType==AluType::ADD) {
		if (imm1 && imm2) {
			int ans = dynamic_cast<ConstantInt *> (op1)->value + dynamic_cast<ConstantInt *> (op2)->value;
			mipsInst = new MipsLiInst(dst, new MipsImm(ans));
		} else {
			MipsOperand *mop1, *mop2;
			if (imm1) {
				mop2 = parseOperand(op1, true);
				mop1 = parseOperand(op2, false);
			} else {
				mop1 = parseOperand(op1, true);
				mop2 = parseOperand(op2, false);
			}
			mipsInst = new MipsBinInst(BinType::M_ADDU, dst, mop1, mop2);
		}
	}
	if (inst->aluType==AluType::SUB) {
		if (imm1 && imm2) {
			int ans = dynamic_cast<ConstantInt *> (op1)->value - dynamic_cast<ConstantInt *> (op2)->value;
			mipsInst = new MipsLiInst(dst, new MipsImm(ans));
		} else {
			MipsOperand *mop1, *mop2;
			if (imm1) {
				mop2 = parseOperand(op1, true);
				mop1 = parseOperand(op2, false);
			} else {
				mop1 = parseOperand(op1, true);
				mop2 = parseOperand(op2, false);
			}
			mipsInst = new MipsBinInst(BinType::M_SUBU, dst, mop1, mop2);
		}
	}
	if (inst->aluType==AluType::MUL) {
		if (imm1 && imm2) {
			int ans = dynamic_cast<ConstantInt *> (op1)->value*dynamic_cast<ConstantInt *> (op2)->value;
			mipsInst = new MipsLiInst(dst, new MipsImm(ans));
		} else {
			MipsOperand *mop1, *mop2;
			if (imm1) {
				mop2 = parseOperand(op1, true);
				mop1 = parseOperand(op2, false);
			} else {
				mop1 = parseOperand(op1, true);
				mop2 = parseOperand(op2, false);
			}
			mipsInst = new MipsBinInst(BinType::M_MUL, dst, mop1, mop2);
		}
	}
	if (inst->aluType==AluType::SDIV) {
		if (imm1 && imm2) {
			int ans = dynamic_cast<ConstantInt *> (op1)->value/dynamic_cast<ConstantInt *> (op2)->value;
			mipsInst = new MipsLiInst(dst, new MipsImm(ans));
		} else {
			MipsOperand *mop1, *mop2;
			if (imm1) {
				mop2 = parseOperand(op1, true);
				mop1 = parseOperand(op2, false);
			} else {
				mop1 = parseOperand(op1, true);
				mop2 = parseOperand(op2, false);
			}
			mipsInst = new MipsBinInst(BinType::M_DIV, dst, mop1, mop2);
		}
	}
	if (inst->aluType==AluType::SREM) {
		if (imm1 && imm2) {
			int ans = dynamic_cast<ConstantInt *> (op1)->value%dynamic_cast<ConstantInt *> (op2)->value;
			mipsInst = new MipsLiInst(dst, new MipsImm(ans));
		} else {
			MipsOperand *mop1, *mop2;
			if (imm1) {
				mop2 = parseOperand(op1, true);
				mop1 = parseOperand(op2, false);
			} else {
				mop1 = parseOperand(op1, true);
				mop2 = parseOperand(op2, false);
			}
			mipsInst = new MipsBinInst(BinType::M_SREM, dst, mop1, mop2);
		}
	}
	curMipsBlock->addInst(mipsInst);
}

void MipsParser::parseBrInst(BrInst *inst) {
	if (inst->jump) {
		auto *br = new MipsBranchInst(CondType::J, new MipsLabel(inst->getOp(0)->name));
		curMipsBlock->addInst(br);
	} else {
		auto mop = parseOperand(inst->getOp(0), false);
		auto *br = new MipsBranchInst(CondType::BNE, mop, $zero, new MipsLabel(inst->getOp(1)->name));
		auto *br1 = new MipsBranchInst(CondType::J, new MipsLabel(inst->getOp(2)->name));

		curMipsBlock->addInst(br);
		curMipsBlock->addInst(br1);
	}
}

void MipsParser::parseCallInst(CallInst *inst) {

	auto *func = (Function *)inst->getOp(0);

	if (func->isLink) {

//		mipsBuilder->buildLiInst($a0, )
		if (func==Function::getint) {
			mipsBuilder->curBlock->instructionList.push_back(new MipsMarco("getint"));
		}
		if (func==Function::putint) {
			mipsBuilder->allocate(inst->getOp(1), $a0);
			mipsBuilder->curBlock->instructionList.push_back(new MipsMarco("putint"));
		}
		if (func==Function::putstr) {
			mipsBuilder->allocate(inst->getOp(1), $a0);
			mipsBuilder->curBlock->instructionList.push_back(new MipsMarco("putstr"));
		}

		if (inst->type!=IntegerType::VOID) {
			mipsBuilder->allocateAndStore(inst, $v0);
		}
		return;
	}

	int size = (int)inst->opList.size() - 1;

	mipsBuilder->stackOffset -= 8;
	mipsBuilder->buildStoreInst($sp, new MipsImm(mipsBuilder->stackOffset + 4), $sp);
	mipsBuilder->buildStoreInst($ra, new MipsImm(mipsBuilder->stackOffset), $sp);

	if (size > 0) {
		int argOff = 0;
		for (int i = 1; i <= size; ++i) {
			argOff -= 4;
			mipsBuilder->allocate(inst->opList[i], $t0);

//			mipsBuilder->stackOffset-=4;
			mipsBuilder->buildStoreInst($t0, new MipsImm(mipsBuilder->stackOffset + argOff), $sp);
//			mipsBuilder->allocateAndStore(inst->operandList[i], $t0);
		}
	}

	mipsBuilder->buildBinInst(M_ADDU, $sp, $sp, new MipsImm(mipsBuilder->stackOffset));
	// give it fuck;
	mipsBuilder->buildBranchInst(JAL, new MipsLabel("Function_" + func->name.substr(1)));

	mipsBuilder->buildLoadInst($ra, new MipsImm(0), $sp);
	mipsBuilder->buildLoadInst($sp, new MipsImm(4), $sp);
	mipsBuilder->stackOffset += 8;

	if (inst->type!=IntegerType::VOID) {
		mipsBuilder->allocateAndStore(inst, $v0);
	}

}

void MipsParser::parseGEPInst(GEPInst *inst) {
	auto op1 = inst->getOp(0);
	mipsBuilder->allocate(op1, $t0);
	if (inst->getOp(2)) {
		mipsBuilder->allocate(inst->getOp(2), $t1);
		mipsBuilder->buildBinInst(M_SLL, $t1, $t1, new MipsImm(2));
		mipsBuilder->buildBinInst(M_ADDU, $t0, $t0, $t1);
	} else {
		mipsBuilder->allocate(inst->getOp(1), $t1);
		mipsBuilder->buildBinInst(M_SLL, $t1, $t1, new MipsImm(2));
		mipsBuilder->buildBinInst(M_ADDU, $t0, $t0, $t1);
	}

//	mipsBuilder->buildLoadInst($t0, new MipsImm(0), $t0);
	mipsBuilder->allocateAndStore(inst, $t0);
}

void MipsParser::parseIcmpInst(IcmpInst *inst) {
	auto op1 = inst->getOp(0), op2 = inst->getOp(1);

	auto mop1 = parseOperand(op1, false), mop2 = parseOperand(op2, false);
	auto dst = parseOperand(inst, false);
	MipsInst *mipsInst;

	switch (inst->icmpType) {
	case IcmpType::EQ: mipsInst = new MipsCmpInst(BEQ, dst, mop1, mop2);
		break;
	case IcmpType::NE: mipsInst = new MipsCmpInst(BNE, dst, mop1, mop2);
		break;
	case IcmpType::LE: mipsInst = new MipsCmpInst(BLE, dst, mop1, mop2);
		break;
	case IcmpType::LT:mipsInst = new MipsCmpInst(BLT, dst, mop1, mop2);
		break;
	case IcmpType::GE:mipsInst = new MipsCmpInst(BGE, dst, mop1, mop2);
		break;
	case IcmpType::GT:mipsInst = new MipsCmpInst(BGT, dst, mop1, mop2);
		break;
	default:break;
	}
	curMipsBlock->addInst(mipsInst);
}

void MipsParser::parseLoadInst(LoadInst *inst) {
	auto op1 = inst->getOp(0);

	mipsBuilder->allocate(op1, $t1);
	mipsBuilder->buildLoadInst($t0, new MipsImm(0), $t1);
	mipsBuilder->allocateAndStore(inst, $t0);

}

void MipsParser::parseReturnInst(ReturnInst *inst) {
	if (inst->type!=IntegerType::VOID) {
		auto *mop = parseOperand(inst->getOp(0), true);
		auto *m = new MipsLiInst($v0, mop);
	}
	auto *mipsInst = new MipsBranchInst(CondType::JR, $ra, nullptr, nullptr);
	curMipsBlock->addInst(mipsInst);
}

void MipsParser::parseStoreInst(StoreInst *inst) {
	auto op1 = inst->getOp(0), op2 = inst->getOp(1);

	mipsBuilder->allocate(op1, $t0);

	mipsBuilder->allocate(op2, $t1);

	mipsBuilder->buildStoreInst($t0, new MipsImm(0), $t1);
}

void MipsParser::parseZextInst(ZextInst *inst) {
	auto mop1 = parseOperand(inst, false);
	auto mop2 = parseOperand(inst->getOp(0), true);

	curMipsBlock->addInst(new MipsLiInst(mop1, mop2));
}

void MipsParser::parseMoveInst(MoveInst *inst) {
	auto mop1 = parseOperand(inst->dst, false);
	auto mop2 = parseOperand(inst->src, true);

	curMipsBlock->addInst(new MipsLiInst(mop1, mop2));
}
