//
// Created by hiccup on 2023/11/14.
//

#pragma once

#include <map>
#include "../llvm-ir/Value.h"
#include "MipsComponent.h"
#include "../llvm-ir/constant/Constant.h"
#include "../llvm-ir/constant/ConstantInt.h"
#include "../llvm-ir/Function.h"
#include "../llvm-ir/BasicBlock.h"

class MipsBuilder {
public:
	std::map<Value *, int> *value2Offset = new map<Value *, int>;
	int stackOffset = 0;
	int dataOffset = 0;
	MipsFunction *curFunc;
	MipsBlock *curBlock;
	MipsModule *mipsModule = new MipsModule();

public:
	void buildFunc(Function *function) {
		auto *func = new MipsFunction();
		mipsModule->functions.push_back(func);
		func->name = function->name;

		curFunc = func;
		value2Offset = new map<Value *, int>;
		stackOffset = 0;
	}

	void buildBlock(BasicBlock *bb) {
		auto *block = new MipsBlock();
		curFunc->blockList.push_back(block);
		curBlock = block;
		block->label = bb->name;
	}

	void buildBinInst(BinType ty, MipsOperand *dst, MipsOperand *src1, MipsOperand *src2) {
		auto *bin = new MipsBinInst();
		bin->src1 = src1;
		bin->src2 = src2;
		bin->dst = dst;
		bin->ty = ty;
		curBlock->instructionList.push_back(bin);
	}

	void buildLiInst(MipsOperand *dst, MipsOperand *imm) {
		auto *li = new MipsLiInst(dst, imm);
		curBlock->instructionList.push_back(li);
	}

	void buildMoveInst(MipsOperand *dst, MipsOperand *src1) {
		auto *move = new MipsMoveInst();
		move->src1 = src1;
		move->dst = dst;
		curBlock->instructionList.push_back(move);
	}

	void buildBranchInst(CondType ty, MipsOperand *src1, MipsOperand *src2, MipsLabel *label) {
		auto *branch = new MipsBranchInst(ty, src1, src2, label);
		curBlock->instructionList.push_back(branch);
	}

	void buildBranchInst(CondType ty, MipsLabel *label) {
		auto *branch = new MipsBranchInst(ty, nullptr, nullptr, label);
		curBlock->instructionList.push_back(branch);
	}


	void buildCmpInst(CondType ty, MipsOperand *dst, MipsOperand *src1, MipsOperand *src2) {
		auto *cmp = new MipsCmpInst();
		cmp->ty = ty;
		cmp->src1 = src1;
		cmp->src2 = src2;
		cmp->dst = dst;
		curBlock->instructionList.push_back(cmp);
	}

	void buildStoreInst(MipsOperand *dst, MipsOperand *addr, MipsOperand *offset) {
		auto *store = new MipsStoreInst(dst, addr, offset);
		curBlock->instructionList.push_back(store);
	}

	void buildLoadInst(MipsOperand *dst, MipsOperand *addr, MipsOperand *offset) {
		auto *load = new MipsLoadInst(dst, addr, offset);
		curBlock->instructionList.push_back(load);
	}

	void buildCommentInst(std::string comment) {
		auto *commentInst = new MipsComment(comment);
		curBlock->instructionList.push_back(commentInst);
	}

	int checkStackExistence(Value *val) {
		if (value2Offset->count(val)) {
			return (*value2Offset)[val];
		} else {
			stackOffset -= 4;
			(*value2Offset)[val] = stackOffset;
			return stackOffset;
		}
	}

	void allocate(Value *val, MipsOperand* reg) {
		if (dynamic_cast<Constant *>(val)) {
			buildLiInst(reg, new MipsImm(((ConstantInt *)val)->value));
		} else if (val->isGlobal()) {
			int off = checkStackExistence(val);
			buildLoadInst(reg, new MipsImm(off), $zero);
		} else {
			int off = checkStackExistence(val);
			buildLoadInst(reg, new MipsImm(off), $sp);
		}
	}

	void allocateAndStore(Value *val, MipsOperand* reg) {
		if (val->isGlobal()) {
			int off = checkStackExistence(val);
			buildStoreInst(reg, new MipsImm(off), $zero);
		} else {
			int off = checkStackExistence(val);
			buildStoreInst(reg, new MipsImm(off), $sp);
		}
	}

};




