//
// Created by hiccup on 2023/11/14.
//

#pragma once

#include "../llvm-ir/Function.h"
#include "../llvm-ir/constant/ConstantInt.h"
#include "mips/MipsComponent.h"
#include "mips/MipsInst.h"
#include <unordered_map>

class Module;
class Function;
class GlobalVariable;
class Instruction;
class BasicBlock;
class AllocaInst;
class AluInst;
class CallInst;
class BrInst;
class IcmpInst;
class ReturnInst;
class ZextInst;
class LoadInst;
class GEPInst;
class StoreInst;
class MoveInst;

/*
 * 	the existence of MipsParser is to make sure we use the machine code efficiently
 *	but not to allocate all the registers or context saving !!
 */


class MipsParser {
public:
	Module *module;
	MipsModule *mipsModule;

	MipsFunction *curMipsFunction{};
	MipsBlock *curMipsBlock{};

	std::unordered_map<GlobalVariable *, MipsOperand *> *global2Operand;
	std::unordered_map<Value *, MipsOperand *> *value2Operand;

	std::unordered_map<BasicBlock *, MipsBlock*> *llvm2MipsBlock;



	int N=32;
	int log = 0;
	int shift=0;
	long long multiplier=0;

public:
	explicit MipsParser(Module *mod) {
		module = mod;
		mipsModule = new MipsModule;
		global2Operand = new unordered_map<GlobalVariable *, MipsOperand *>;
		value2Operand = new unordered_map<Value *, MipsOperand *>;
		llvm2MipsBlock = new unordered_map<BasicBlock *, MipsBlock*>;
	}

	void parseModule();
	void parseGlobalVar(GlobalVariable *) const;
	void parseFunction(Function *);
	void parseInstruction(Instruction *, MipsBlock *);

	void parseAllocaInst(AllocaInst *inst) const;
	void parseAluInst(AluInst *inst);
	void parseCallInst(CallInst *inst) const;
	void parseBrInst(BrInst *inst) const;
	void parseIcmpInst(IcmpInst *inst) const;
	void parseReturnInst(ReturnInst *inst) const;
	void parseZextInst(ZextInst *inst) const;
	void parseGEPInst(GEPInst *inst) const;
	void parseLoadInst(LoadInst *inst) const;
	void parseStoreInst(StoreInst *inst) const;
	void parseMoveInst(MoveInst *inst) const;
	MipsOperand *parseOp(Value *val, bool canImm) const;
	MipsOperand *parseGlobalOp(GlobalVariable *val, bool imm) const;
	MipsOperand *parseArgumentOp(Argument *val, bool imm) const;
	MipsOperand *parseConstOp(ConstantInt *val, bool imm) const;

	void chooseMultiplier(int divisor);
	void divide(MipsReg *dividend, MipsReg* target, int divisor);
};