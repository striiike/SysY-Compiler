//
// Created by hiccup on 2023/11/14.
//

#include "MipsParser.h"
#include "../llvm-ir/Module.h"
#include "../llvm-ir/Function.h"
#include "../llvm-ir/BasicBlock.h"
#include "../llvm-ir/GlobalVariable.h"
#include "../llvm-ir/constant/Constant.h"
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

	(*mipsBuilder->value2Offset)[glo] = mipsBuilder->dataOffset;
	if (glo->init->isArr()) {
		auto item = ((ConstantArray *)glo->init);
		mipsGlo->arr = item->array;
		mipsBuilder->dataOffset += item->array.size()*4;
	}
	if (glo->init->isInt()) {
		auto item = ((ConstantInt *)glo->init);
		mipsGlo->arr.push_back(item->value);
		mipsBuilder->dataOffset += 4;
	}
	if (glo->init->isStr()) {
		auto item = ((ConstantString *)glo->init);
		mipsGlo->stringLiteral = item->str;
		mipsBuilder->dataOffset += item->str.size();
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

//	mipsBuilder->buildBinInst(BinType::M_SUBU, $sp, $sp, new MipsImm(len));
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

}
void MipsParser::parseGEPInst(GEPInst *inst) {

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
}
void MipsParser::parseLoadInst(LoadInst *inst) {
	auto op1 = inst->getOperand(0);

	mipsBuilder->allocate(op1, $t0);

	mipsBuilder->allocateAndStore(inst, $t0);

}
void MipsParser::parseReturnInst(ReturnInst *inst) {
	MipsOperand *src = nullptr;
	if (inst->type==IntegerType::VOID) {

	} else {
		auto op1 = inst->getOperand(0);
		mipsBuilder->allocate(op1, $t0);
		src = $t0;
	}

	mipsBuilder->buildBranchInst(CondType::JR, src, nullptr, nullptr);
}
void MipsParser::parseStoreInst(StoreInst *inst) {
	auto op1 = inst->getOperand(0), op2 = inst->getOperand(1);

	mipsBuilder->allocate(op1, $t0);

	mipsBuilder->allocateAndStore(op2, $t1);

}
void MipsParser::parseZextInst(ZextInst *inst) {

}
void MipsParser::parsePhiInst(PhiInst *inst) {

}
