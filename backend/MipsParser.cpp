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
#include "../llvm-ir/instruction/PhiInst.h"

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

	(*mipsBuilder->global2Label)[glo] = mipsGlo->name;
	if (glo->init->isArr()) {
		auto item = ((ConstantArray *)glo->init);
		mipsGlo->arr = item->array;
		mipsBuilder->dataOffset += mipsGlo->size = glo->getSize();
	}
	if (glo->init->isInt()) {
		auto item = ((ConstantInt *)glo->init);
		mipsGlo->arr.push_back(item->value);
		mipsBuilder->dataOffset += 4;
	}
	if (glo->init->isStr()) {
		auto item = ((ConstantString *)glo->init);
		mipsGlo->stringLiteral = item->mipsString();
		mipsBuilder->dataOffset += mipsGlo->stringLiteral.size();
	}
	mipsBuilder->mipsModule->globals.push_back(mipsGlo);

}

void MipsParser::parseFunction(Function *func) {
	mipsBuilder->buildFunc(func);
	for (auto i : func->basicList) {
		mipsBuilder->buildBlock(i);
		for (auto j : i->instructionList) {
			parseInstruction(j);
		}
	}
}

void MipsParser::parseInstruction(Instruction *inst) {
	mipsBuilder->curBlock->instructionList.push_back(new MipsComment(inst->toString()));
	switch (inst->instType) {
	case InstType::ALLOCA: parseAllocaInst((AllocaInst *)inst);
		break;
	case InstType::ALU: parseAluInst((AluInst *)inst);
		break;
	case InstType::BRANCH: parseBrInst((BrInst *)inst);
		break;
	case InstType::CALL: parseCallInst((CallInst *)inst);
		break;
	case InstType::GEP: parseGEPInst((GEPInst *)inst);
		break;
	case InstType::ICMP: parseIcmpInst((IcmpInst *)inst);
		break;
	case InstType::LOAD: parseLoadInst((LoadInst *)inst);
		break;
	case InstType::RETURN: parseReturnInst((ReturnInst *)inst);
		break;
	case InstType::STORE: parseStoreInst((StoreInst *)inst);
		break;
	case InstType::PHI: parsePhiInst((PhiInst *)inst);
		break;
	case InstType::ZEXT: parseZextInst((ZextInst *)inst);
		break;
	default: break;
	}
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

	auto op1 = inst->getOperand(0), op2 = inst->getOperand(1);

	mipsBuilder->allocate(op1, $t0);
	mipsBuilder->allocate(op2, $t1);

	switch (inst->aluType) {
	case AluType::ADD: mipsBuilder->buildBinInst(M_ADDU, $t0, $t0, $t1);
		break;
	case AluType::SUB: mipsBuilder->buildBinInst(M_SUBU, $t0, $t0, $t1);
		break;
	case AluType::MUL: mipsBuilder->buildBinInst(M_MUL, $t0, $t0, $t1);
		break;
	case AluType::SDIV:mipsBuilder->buildBinInst(M_DIV, $t0, $t0, $t1);
		break;
	case AluType::SREM:mipsBuilder->buildBinInst(M_SREM, $t0, $t0, $t1);
		break;
	default:break;
	}

	mipsBuilder->allocateAndStore(inst, $t0);

}

void MipsParser::parseBrInst(BrInst *inst) {
	if (inst->jump) {
		mipsBuilder->buildBranchInst(CondType::J, new MipsLabel(inst->getOperand(0)->name));
	} else {
		mipsBuilder->allocate(inst->getOperand(0), $t0);

		mipsBuilder->buildBranchInst(CondType::BNE, $t0, $zero, new MipsLabel(inst->getOperand(1)->name));
		mipsBuilder->buildBranchInst(CondType::J, new MipsLabel(inst->getOperand(2)->name));
	}

}

void MipsParser::parseCallInst(CallInst *inst) {

	auto *func = (Function *)inst->getOperand(0);

	if (func->isLink) {

//		mipsBuilder->buildLiInst($a0, )
		if (func==Function::getint) {
			mipsBuilder->curBlock->instructionList.push_back(new MipsMarco("getint"));
		}
		if (func==Function::putint) {
			mipsBuilder->allocate(inst->getOperand(1), $a0);
			mipsBuilder->curBlock->instructionList.push_back(new MipsMarco("putint"));
		}
		if (func==Function::putstr) {
			mipsBuilder->allocate(inst->getOperand(1), $a0);
			mipsBuilder->curBlock->instructionList.push_back(new MipsMarco("putstr"));
		}

		if (inst->type!=IntegerType::VOID) {
			mipsBuilder->allocateAndStore(inst, $v0);
		}
		return;
	}

	int size = (int)inst->operandList.size() - 1;

	mipsBuilder->stackOffset -= 8;
	mipsBuilder->buildStoreInst($sp, new MipsImm(mipsBuilder->stackOffset + 4), $sp);
	mipsBuilder->buildStoreInst($ra, new MipsImm(mipsBuilder->stackOffset), $sp);

	if (size > 0) {
		int argOff = 0;
		for (int i = 1; i <= size; ++i) {
			argOff -= 4;
			mipsBuilder->allocate(inst->operandList[i], $t0);

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
	auto op1 = inst->getOperand(0);
	mipsBuilder->allocate(op1, $t0);
	if (inst->getOperand(2)) {
		mipsBuilder->allocate(inst->getOperand(2), $t1);
		mipsBuilder->buildBinInst(M_SLL, $t1, $t1, new MipsImm(2));
		mipsBuilder->buildBinInst(M_ADDU, $t0, $t0, $t1);
	} else {
		mipsBuilder->allocate(inst->getOperand(1), $t1);
		mipsBuilder->buildBinInst(M_SLL, $t1, $t1, new MipsImm(2));
		mipsBuilder->buildBinInst(M_ADDU, $t0, $t0, $t1);
	}

//	mipsBuilder->buildLoadInst($t0, new MipsImm(0), $t0);
	mipsBuilder->allocateAndStore(inst, $t0);
}

void MipsParser::parseIcmpInst(IcmpInst *inst) {
	auto op1 = inst->getOperand(0), op2 = inst->getOperand(1);

	mipsBuilder->allocate(op1, $t0);
	mipsBuilder->allocate(op2, $t1);

	switch (inst->icmpType) {
	case IcmpType::EQ: mipsBuilder->buildCmpInst(BEQ, $t0, $t0, $t1);
		break;
	case IcmpType::NE: mipsBuilder->buildCmpInst(BNE, $t0, $t0, $t1);
		break;
	case IcmpType::LE: mipsBuilder->buildCmpInst(BLE, $t0, $t0, $t1);
		break;
	case IcmpType::LT:mipsBuilder->buildCmpInst(BLT, $t0, $t0, $t1);
		break;
	case IcmpType::GE:mipsBuilder->buildCmpInst(BGE, $t0, $t0, $t1);
		break;
	case IcmpType::GT:mipsBuilder->buildCmpInst(BGT, $t0, $t0, $t1);
		break;
	default:break;
	}
	mipsBuilder->allocateAndStore(inst, $t0);
}

void MipsParser::parseLoadInst(LoadInst *inst) {
	auto op1 = inst->getOperand(0);

	mipsBuilder->allocate(op1, $t1);
	mipsBuilder->buildLoadInst($t0, new MipsImm(0), $t1);
	mipsBuilder->allocateAndStore(inst, $t0);

}

void MipsParser::parseReturnInst(ReturnInst *inst) {
	MipsOperand *src = nullptr;
	if (inst->type==IntegerType::VOID) {

	} else {
		auto op1 = inst->getOperand(0);
		mipsBuilder->allocate(op1, $v0);
//		src = $t0;
	}

//	mipsBuilder->buildLoadInst($sp, new MipsImm(-4), $sp);
//	mipsBuilder->buildLoadInst($ra, new MipsImm(-8), $sp);

	mipsBuilder->buildBranchInst(CondType::JR, $ra, nullptr, nullptr);
}

void MipsParser::parseStoreInst(StoreInst *inst) {
	auto op1 = inst->getOperand(0), op2 = inst->getOperand(1);

	mipsBuilder->allocate(op1, $t0);

	mipsBuilder->allocate(op2, $t1);

	mipsBuilder->buildStoreInst($t0, new MipsImm(0), $t1);
}

void MipsParser::parseZextInst(ZextInst *inst) {
	mipsBuilder->allocate(inst->getOperand(0), $t0);
	mipsBuilder->allocateAndStore(inst, $t0);
}
void MipsParser::parsePhiInst(PhiInst *inst) {

}
