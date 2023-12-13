//
// Created by hiccup on 2023/11/14.
//

#include "MipsParser.h"
#include "../llvm-ir/Module.h"
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
		mipsGlo->size = ((ArrayType *)item->type)->getNum()*4;
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
	curMipsFunction = new MipsFunction(func->name.substr(1));
	mipsModule->functions.push_back(curMipsFunction);

	for (auto i : func->basicList) {

		curMipsBlock = new MipsBlock(i->name);
		curMipsFunction->blockList.push_back(curMipsBlock);
		(*llvm2MipsBlock)[i] = curMipsBlock;

		for (auto j : i->instructionList) {
			parseInstruction(j, curMipsBlock);
		}
	}

	/*
	 * make CFG in mipsBlock, they are the same
	 */
	for (auto i : func->basicList) {
		auto curMips = (*llvm2MipsBlock)[i];
		for (auto j : *i->preBbs) {
			curMips->pred->insert((*llvm2MipsBlock)[j]);
		}
		for (auto j : *i->sucBbs) {
			curMips->succ->insert((*llvm2MipsBlock)[j]);
		}
	}
}

void MipsParser::parseInstruction(Instruction *inst, MipsBlock *block) {
	block->instList.push_back(new MipsComment(inst->toString()));
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
MipsOperand *MipsParser::parseOp(Value *val, bool canImm) const {
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

	if (dynamic_cast<GlobalVariable *>(val)) {
		return parseGlobalOp(dynamic_cast<GlobalVariable *>(val), canImm);
	} else if (dynamic_cast<Argument *>(val)) {
		return parseArgumentOp(dynamic_cast<Argument *>(val), canImm);
	} else if (dynamic_cast<ConstantInt *>(val)) {
		return parseConstOp(dynamic_cast<ConstantInt *>(val), canImm);
	} else {
		auto *vr = new MipsVrReg();
		(*value2Operand)[val] = vr;
		return vr;
	}
}

MipsOperand *MipsParser::parseGlobalOp(GlobalVariable *val, bool canImm) const {
	auto *dst = new MipsVrReg();
	auto *inst = new MipsLiInst(dst, (*global2Operand)[val]);
	curMipsBlock->addInst(inst);
	return dst;
}

MipsOperand *MipsParser::parseArgumentOp(Argument *val, bool canImm) const {
	/*
	 *  It has to be inserted to the first block.
	 *  because if you firstly use in the 2nd, and reuse it in the 3rd, that would be wrong.
	 */
	int rk = val->rank;
	auto *dst = new MipsVrReg();
	if (rk < 4) {
		auto *inst = new MipsLiInst(dst, (rk==0) ? $a0 : (rk==1) ? $a1 : (rk==2) ? $a2 : (rk==3) ? $a3 : nullptr);
//		curMipsBlock->addInst(inst);
		curMipsFunction->addArgToFront(inst);
	} else {
		auto *m1 = new MipsLoadInst(dst, new MipsImm((rk - 4)*4), $sp);
//		curMipsBlock->addInst(m1);
		curMipsFunction->addArgToFront(m1);
	}
	(*value2Operand)[val] = dst;
	return dst;
}

MipsOperand *MipsParser::parseConstOp(ConstantInt *val, bool canImm) const {
	if (!canImm) {
		auto *tmp = new MipsVrReg();
		auto *inst = new MipsLiInst(tmp, new MipsImm(val->value));
		curMipsBlock->addInst(inst);
		return tmp;
	}
	return new MipsImm(val->value);
}

void MipsParser::parseAllocaInst(AllocaInst *inst) const {
	auto *dst = parseOp(inst, false);
	(*value2Operand)[inst] = dst;
	auto mipsInst = new MipsBinInst(BinType::M_ADDU, dst, $sp, new MipsImm(curMipsFunction->stackOff));
	curMipsBlock->addInst(mipsInst);

	int len;
	if (inst->retType==IntegerType::INT32) {
		len = 4;
		curMipsFunction->stackOff += 4;
	} else {
		len = ((ArrayType *)inst->retType)->getNum();
		curMipsFunction->stackOff += 4*len;
	}
}

void MipsParser::chooseMultiplier(int divisor) {
	log = N - __builtin_clz(divisor - 1);
	shift = log;
	long long low = (1LL << (N + shift))/divisor;
	long long high = ((1LL << (N + shift)) + (1LL << (shift + 1)))/divisor;

	while ((low >> 1) < (high >> 1) && shift > 0) {
		low >>= 1;
		high >>= 1;
		shift--;
	}
	multiplier = high;
}

void MipsParser::divide(MipsReg *target, MipsReg *dividend, int divisor) {
	bool isNegative = (divisor < 0);
	int divisorAbs = isNegative ? -divisor : divisor;
	chooseMultiplier(divisorAbs);
	if (divisorAbs==1) {
		if (dividend!=target) {
			curMipsBlock->addInst(new MipsBinInst(M_ADDU, target, $zero, dividend));
		}
	} else if (divisorAbs==(1 << log)) {
		auto *vr0 = new MipsVrReg();
		curMipsBlock->addInst(new MipsBinInst(M_SRA, vr0, dividend, new MipsImm(log - 1)));
		curMipsBlock->addInst(new MipsBinInst(M_SRL, vr0, vr0, new MipsImm(N - log)));
		curMipsBlock->addInst(new MipsBinInst(M_ADDU, vr0, vr0, dividend));
		curMipsBlock->addInst(new MipsBinInst(M_SRA, target, vr0, new MipsImm(log)));

	} else if (((unsigned long long)multiplier) < 0x80000000LL) {
		auto *vr0 = new MipsVrReg(), *vr1 = new MipsVrReg();
		int m = (int)multiplier;
		curMipsBlock->addInst(new MipsLiInst(vr0, new MipsImm(m)));
		curMipsBlock->addInst(new MipsMULT(vr0, dividend));
		curMipsBlock->addInst(new MipsMFHI(vr0));
		curMipsBlock->addInst(new MipsBinInst(M_SRA, vr0, vr0, new MipsImm(shift)));
		curMipsBlock->addInst(new MipsBinInst(M_SRA, vr1, dividend, new MipsImm(31)));
		curMipsBlock->addInst(new MipsBinInst(M_SUBU, target, vr0, vr1));

	} else {
		auto *vr0 = new MipsVrReg(), *vr1 = new MipsVrReg();
		int m = (int)(multiplier - 0x100000000LL);
		curMipsBlock->addInst(new MipsLiInst(vr0, new MipsImm(m)));
		curMipsBlock->addInst(new MipsMULT(vr0, dividend));
		curMipsBlock->addInst(new MipsMFHI(vr0));
		curMipsBlock->addInst(new MipsBinInst(M_ADDU, vr0, vr0, dividend));
		curMipsBlock->addInst(new MipsBinInst(M_SRA, vr0, vr0, new MipsImm(shift)));
		curMipsBlock->addInst(new MipsBinInst(M_SRA, vr1, dividend, new MipsImm(31)));
		curMipsBlock->addInst(new MipsBinInst(M_SUBU, target, vr0, vr1));
	}
	if (isNegative) {
		curMipsBlock->addInst(new MipsBinInst(M_SUBU, target, $zero, target));
	}
}

int findSingleOne(int num) {
	int position = 0;
	int count = 0;

	while (num) {
		if (num & 1) {
			count++;
//			position = count;
		}

		if (count > 1) {
			return -1;
		}
		position++;
		num >>= 1;
	}

	return position-1;
}


void MipsParser::parseAluInst(AluInst *inst) {
	auto op1 = inst->getOp(0), op2 = inst->getOp(1);
	bool imm1 = dynamic_cast<ConstantInt *> (op1);
	bool imm2 = dynamic_cast<ConstantInt *> (op2);

	auto dst = parseOp(inst, false);
	MipsInst *mipsInst;
	if (inst->aluType==AluType::ADD) {
		if (imm1 && imm2) {
			int ans = dynamic_cast<ConstantInt *> (op1)->value + dynamic_cast<ConstantInt *> (op2)->value;
			mipsInst = new MipsLiInst(dst, new MipsImm(ans));
		} else {
			MipsOperand *mop1, *mop2;
			if (imm1) {
				mop2 = parseOp(op1, true);
				mop1 = parseOp(op2, false);
			} else {
				mop1 = parseOp(op1, false);
				mop2 = parseOp(op2, true);
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

			mop1 = parseOp(op1, false);
			mop2 = parseOp(op2, false);

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
				mop2 = parseOp(op1, true);
				mop1 = parseOp(op2, false);
			} else {
				mop1 = parseOp(op1, false);
				mop2 = parseOp(op2, true);
			}

			if (dynamic_cast<MipsImm *>(mop2)) {
				int ones = findSingleOne(dynamic_cast<MipsImm *>(mop2)->imm);
				if (ones!=-1) {
					mipsInst = new MipsBinInst(BinType::M_SLL, dst, mop1, new MipsImm(ones));
					curMipsBlock->addInst(mipsInst);
					return;
				}
			}

			mipsInst = new MipsBinInst(BinType::M_MUL, dst, mop1, mop2);
		}
	}
	if (inst->aluType==AluType::SDIV) {
		if (imm1 && imm2) {
			int ans = dynamic_cast<ConstantInt *> (op1)->value/dynamic_cast<ConstantInt *> (op2)->value;
			mipsInst = new MipsLiInst(dst, new MipsImm(ans));
		} else {

			if (imm2) {
				MipsOperand *mop1, *mop2;
				mop1 = parseOp(op1, false);
				mop2 = parseOp(op2, true);

				divide((MipsReg *)dst, (MipsReg *)mop1, ((MipsImm *)mop2)->imm);

				return;
			} else {

				MipsOperand *mop1, *mop2;
				mop1 = parseOp(op1, false);
				mop2 = parseOp(op2, false);

				mipsInst = new MipsBinInst(BinType::M_DIV, dst, mop1, mop2);
			}

		}
	}
	if (inst->aluType==AluType::SREM) {
		if (imm1 && imm2) {
			int ans = dynamic_cast<ConstantInt *> (op1)->value%dynamic_cast<ConstantInt *> (op2)->value;
			mipsInst = new MipsLiInst(dst, new MipsImm(ans));
		} else {
			MipsOperand *mop1, *mop2;
			if (imm1) {
				mop2 = parseOp(op1, true);
				mop1 = parseOp(op2, false);
			} else {
				mop1 = parseOp(op1, true);
				mop2 = parseOp(op2, false);
			}
			mipsInst = new MipsBinInst(BinType::M_SREM, dst, mop1, mop2);
		}
	}
	curMipsBlock->addInst(mipsInst);
}

void MipsParser::parseBrInst(BrInst *inst) const {
	if (inst->jump) {
		auto *br = new MipsBranchInst(CondType::J, new MipsLabel(inst->getOp(0)->name));
		curMipsBlock->addInst(br);
	} else {
		auto mop = parseOp(inst->getOp(0), false);
		auto *br = new MipsBranchInst(CondType::BNE, mop, $zero, new MipsLabel(inst->getOp(1)->name));
		auto *br1 = new MipsBranchInst(CondType::J, new MipsLabel(inst->getOp(2)->name));

		curMipsBlock->addInst(br);
		curMipsBlock->addInst(br1);
	}
}

void MipsParser::parseCallInst(CallInst *inst) const {

	auto *func = (Function *)inst->getOp(0);
	MipsInst *callInst;

	if (func->isLink) {
		callInst = new MipsMarco(func->name.substr(1));
	} else {
		callInst = new MipsBranchInst(CondType::JAL, new MipsLabel("Function_" + func->name.substr(1)));
	}

	int argsNum = (int)func->argumentList.size();
	for (int i = 0; i < argsNum; ++i) {
		MipsInst *mipsInst;
		if (i < 4) {
			auto mop = parseOp(inst->getOp(i + 1), true);
			mipsInst = new MipsLiInst((i==0) ? $a0 : (i==1) ? $a1 : (i==2) ? $a2 : (i==3) ? $a3 : nullptr, mop);
		} else {
			auto mop = parseOp(inst->getOp(i + 1), false);
			mipsInst = new MipsStoreInst(mop, new MipsImm(-(argsNum - i)*4), $sp);
		}
		curMipsBlock->addInst(mipsInst);
	}

	if (argsNum > 4) {
		auto *tmp = new MipsBinInst(M_ADDU, $sp, $sp, new MipsImm(-(argsNum - 4)*4));
		curMipsBlock->addInst(tmp);
	}
	curMipsBlock->addInst(callInst);
	if (argsNum > 4) {
		auto *tmp = new MipsBinInst(M_ADDU, $sp, $sp, new MipsImm((argsNum - 4)*4));
		curMipsBlock->addInst(tmp);
	}

	if (inst->type!=IntegerType::VOID) {
		auto *dst = parseOp(inst, false);
		auto *m = new MipsLiInst(dst, $v0);
		curMipsBlock->addInst(m);
	}
}

void MipsParser::parseGEPInst(GEPInst *inst) const {
	auto mop1 = parseOp(inst->getOp(0), false);
	MipsOperand *mop2, *dst = parseOp(inst, false);

	Value *_mop2 = (inst->getOp(2)) ? inst->getOp(2) : inst->getOp(1);
	if (dynamic_cast<ConstantInt *>(_mop2)) {
		auto imm = dynamic_cast<ConstantInt *>(_mop2);
		auto mi = new MipsBinInst(M_ADDU, dst, mop1, new MipsImm(imm->value*4));
		curMipsBlock->addInst(mi);
		return;
	}

	if (inst->getOp(2)) {
		mop2 = parseOp(inst->getOp(2), false);
	} else {
		mop2 = parseOp(inst->getOp(1), false);
	}

	auto mul4 = new MipsVrReg();

	auto mi1 = new MipsBinInst(M_SLL, mul4, mop2, new MipsImm(2));
	auto mi2 = new MipsBinInst(M_ADDU, dst, mop1, mul4);
//	auto mipsInst = new MipsLoadInst(dst, new MipsImm(0), mop1);

	curMipsBlock->addInst(mi1);
	curMipsBlock->addInst(mi2);
//	curMipsBlock->addInst(mipsInst);
}

void MipsParser::parseIcmpInst(IcmpInst *inst) const {
	auto op1 = inst->getOp(0), op2 = inst->getOp(1);

	auto mop1 = parseOp(op1, false), mop2 = parseOp(op2, false);
	auto dst = parseOp(inst, false);
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

void MipsParser::parseLoadInst(LoadInst *inst) const {
	auto op1 = inst->getOp(0);

	auto mop1 = parseOp(op1, false), dst = parseOp(inst, false);
	auto mipsInst = new MipsLoadInst(dst, new MipsImm(0), mop1);
	curMipsBlock->addInst(mipsInst);
}

void MipsParser::parseReturnInst(ReturnInst *inst) const {
	if (inst->type!=IntegerType::VOID) {
		auto *mop = parseOp(inst->getOp(0), true);
		auto *m = new MipsLiInst($v0, mop);
		curMipsBlock->addInst(m);
	}
	auto *mipsInst = new MipsBranchInst(CondType::JR, $ra, nullptr, nullptr);
	curMipsBlock->addInst(mipsInst);
}

void MipsParser::parseStoreInst(StoreInst *inst) const {
	auto op1 = inst->getOp(0), op2 = inst->getOp(1);

	auto mop1 = parseOp(op1, false), mop2 = parseOp(op2, false);
	auto mipsInst = new MipsStoreInst(mop1, new MipsImm(0), mop2);
	curMipsBlock->addInst(mipsInst);
}

void MipsParser::parseZextInst(ZextInst *inst) const {
	auto mop1 = parseOp(inst, false);
	auto mop2 = parseOp(inst->getOp(0), true);

	if (dynamic_cast<MipsReg *>(mop2)) {
		curMipsBlock->addInst(new MipsBinInst(M_ADDU, mop1, mop2, new MipsImm(0)));
	} else {
		curMipsBlock->addInst(new MipsLiInst(mop1, mop2));
	}

//	curMipsBlock->addInst(new MipsBinInst(M_ADDU, mop1, $zero, mop2));
}

void MipsParser::parseMoveInst(MoveInst *inst) const {
	auto mop1 = parseOp(inst->dst, false);
	auto mop2 = parseOp(inst->src, true);

//	curMipsBlock->addInst(new MipsLiInst(mop1, mop2));
	if (dynamic_cast<MipsReg *>(mop2)) {
		curMipsBlock->addInst(new MipsBinInst(M_ADDU, mop1, mop2, new MipsImm(0)));
	} else {
		curMipsBlock->addInst(new MipsLiInst(mop1, mop2));
	}
}


/*
 *	-------- view of stack --------
 *
 *	when entering a function
 *
 *		| # 7th arg
 *		| # 6th arg
 *		| # 5th arg
 *		------------  <-- $sp without fixing
 *		|
 *		| context saving where I'm planning
 *		|
 *		------------
 *		|
 *		|
 *		| alloca
 *		|
 *		|
 *		------------
 *		$sp  <--- here is stack pointer
 *
 *	$sp is always at the bottom, in order to reduce calculation
 *
 *
 *
 *
 */

















