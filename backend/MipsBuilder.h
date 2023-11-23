//
// Created by hiccup on 2023/11/14.
//

#pragma once

#include <map>
#include "MipsComponent.h"

class Value;
class Function;
class BasicBlock;

class MipsBuilder {
public:
	std::map<Value *, int> *value2Offset = new map<Value *, int>;
	std::map<Value *, string> *global2Label = new map<Value *, string>;
	int stackOffset = 0;
	int dataOffset = 0;
	MipsFunction *curFunc;
	MipsBlock *curBlock;
	MipsModule *mipsModule = new MipsModule();

public:
	void buildFunc(Function *function);

	void buildBlock(BasicBlock *bb);

	void buildBinInst(BinType ty, MipsOperand *dst, MipsOperand *src1, MipsOperand *src2) const;

	void buildLiInst(MipsOperand *dst, MipsOperand *imm) const;

	void buildMoveInst(MipsOperand *dst, MipsOperand *src1) const;

	void buildBranchInst(CondType ty, MipsOperand *src1, MipsOperand *src2, MipsLabel *label);

	void buildBranchInst(CondType ty, MipsLabel *label);

	void buildCmpInst(CondType ty, MipsOperand *dst, MipsOperand *src1, MipsOperand *src2);

	void buildStoreInst(MipsOperand *dst, MipsOperand *addr, MipsOperand *offset);

	void buildLoadInst(MipsOperand *dst, MipsOperand *addr, MipsOperand *offset);

	void buildCommentInst(std::string comment);

	int checkStackExistence(Value *val);

	void allocate(Value *val, MipsOperand *reg);

	void allocateAndStore(Value *val, MipsOperand *reg);

};




