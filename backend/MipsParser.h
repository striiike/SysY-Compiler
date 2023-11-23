//
// Created by hiccup on 2023/11/14.
//

#pragma once

#include "MipsBuilder.h"
#include "../llvm-ir/Function.h"
#include "../llvm-ir/constant/ConstantInt.h"
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
 *	but not to allocate all the registers !
 */


class MipsParser {
public:
	Module *module;
	MipsModule *mipsModule;

	MipsFunction *curMipsFunction{};
	MipsBlock *curMipsBlock{};

	std::unordered_map<GlobalVariable *, MipsOperand *> *global2Operand;
	std::unordered_map<Value *, MipsOperand *> *value2Operand;

public:
	explicit MipsParser(Module *mod) {
		module = mod;
		mipsModule = new MipsModule;
		global2Operand = new unordered_map<GlobalVariable *, MipsOperand *>;
		value2Operand = new unordered_map<Value *, MipsOperand *>;
	}

	void parseModule();
	void parseGlobalVar(GlobalVariable *) const;
	void parseFunction(Function *);
	void parseInstruction(Instruction *, MipsBlock *);

	void parseAllocaInst(AllocaInst *inst);
	void parseAluInst(AluInst *inst);
	void parseCallInst(CallInst *inst);
	void parseBrInst(BrInst *inst);
	void parseIcmpInst(IcmpInst *inst);
	void parseReturnInst(ReturnInst *inst);
	void parseZextInst(ZextInst *inst);
	void parseGEPInst(GEPInst *inst);
	void parseLoadInst(LoadInst *inst);
	void parseStoreInst(StoreInst *inst);
	void parseMoveInst(MoveInst *inst);
	MipsOperand *parseOperand(Value *val, bool isImm);
	MipsOperand *parseGlobalOp(GlobalVariable *val, bool imm);
	MipsOperand *parseArgumentOp(Argument *val, bool imm);
	MipsOperand *parseConstOp(ConstantInt *val, bool imm);
};