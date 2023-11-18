//
// Created by hiccup on 2023/11/14.
//

#pragma once

#include "MipsBuilder.h"
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
class PhiInst;

class MipsParser {
public:
	Module *module;
	MipsBuilder *mipsBuilder;

public:
	MipsParser(Module *mod) {
		module = mod;
		mipsBuilder = new MipsBuilder();
	}

	void parseModule();
	void parseGlobalVar(GlobalVariable *) const;
	void parseFunction(Function *);
	void parseInstruction(Instruction *);

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
	void parsePhiInst(PhiInst *inst);
};