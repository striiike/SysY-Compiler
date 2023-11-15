//
// Created by hiccup on 2023/11/14.
//

#pragma once

#include "MipsComponent.h"
#include "MipsBuilder.h"
#include "../llvm-ir/instruction/AllocaInst.h"
#include "../llvm-ir/instruction/AluInst.h"
#include "../llvm-ir/instruction/CallInst.h"
#include "../llvm-ir/instruction/BrInst.h"
#include "../llvm-ir/instruction/IcmpInst.h"
#include "../llvm-ir/instruction/ReturnInst.h"
#include "../llvm-ir/instruction/ZextInst.h"
#include "../llvm-ir/instruction/GEPInst.h"
#include "../llvm-ir/instruction/LoadInst.h"
#include "../llvm-ir/instruction/StoreInst.h"
#include "../llvm-ir/instruction/PhiInst.h"
class Module;
class Function;
class GlobalVariable;
class Instruction;
class BasicBlock;

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