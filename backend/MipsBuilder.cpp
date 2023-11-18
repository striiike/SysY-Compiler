//
// Created by hiccup on 2023/11/14.
//

#include "MipsBuilder.h"
#include "../llvm-ir/Function.h"
#include "../llvm-ir/constant/Constant.h"
#include "../llvm-ir/constant/ConstantInt.h"


void MipsBuilder::buildFunc(Function *function) {
	auto *func = new MipsFunction();
	mipsModule->functions.push_back(func);
	func->name = function->name.substr(1);
	curFunc = func;

	stackOffset = 0;
	value2Offset = new map<Value *, int>;

	for (auto arg : function->argumentList) {
		stackOffset -= 4;
		(*value2Offset)[arg] = stackOffset;
	}

//		curBlock = new MipsBlock();
//		curFunc->blockList.push_back(curBlock);
//		curBlock->label = "context";
//
//		buildBinInst(BinType::M_ADDU, $sp, $sp, new MipsImm(stackOffset));
//		stackOffset = -8;
//		buildStoreInst($sp, new MipsImm(-4), $sp);
//		buildStoreInst($ra, new MipsImm(-8), $sp);
}
void MipsBuilder::buildBlock(BasicBlock *bb) {
	auto *block = new MipsBlock();
	curFunc->blockList.push_back(block);
	curBlock = block;
	block->label = bb->name;
}
void MipsBuilder::buildBinInst(BinType ty, MipsOperand *dst, MipsOperand *src1, MipsOperand *src2) const {
	auto *bin = new MipsBinInst();
	bin->src1 = src1;
	bin->src2 = src2;
	bin->dst = dst;
	bin->ty = ty;
	curBlock->instructionList.push_back(bin);
}
void MipsBuilder::buildLiInst(MipsOperand *dst, MipsOperand *imm) const {
	auto *li = new MipsLiInst(dst, imm);
	curBlock->instructionList.push_back(li);
}
void MipsBuilder::buildMoveInst(MipsOperand *dst, MipsOperand *src1) const {
	auto *move = new MipsMoveInst();
	move->src1 = src1;
	move->dst = dst;
	curBlock->instructionList.push_back(move);
}
void MipsBuilder::buildBranchInst(CondType ty, MipsOperand *src1, MipsOperand *src2, MipsLabel *label) {
	auto *branch = new MipsBranchInst(ty, src1, src2, label);
	curBlock->instructionList.push_back(branch);
}
void MipsBuilder::buildBranchInst(CondType ty, MipsLabel *label) {
	auto *branch = new MipsBranchInst(ty, nullptr, nullptr, label);
	curBlock->instructionList.push_back(branch);
}
void MipsBuilder::buildCmpInst(CondType ty, MipsOperand *dst, MipsOperand *src1, MipsOperand *src2) {
	auto *cmp = new MipsCmpInst();
	cmp->ty = ty;
	cmp->src1 = src1;
	cmp->src2 = src2;
	cmp->dst = dst;
	curBlock->instructionList.push_back(cmp);
}
void MipsBuilder::buildStoreInst(MipsOperand *dst, MipsOperand *addr, MipsOperand *offset) {
	auto *store = new MipsStoreInst(dst, addr, offset);
	curBlock->instructionList.push_back(store);
}
void MipsBuilder::buildLoadInst(MipsOperand *dst, MipsOperand *addr, MipsOperand *offset) {
	auto *load = new MipsLoadInst(dst, addr, offset);
	curBlock->instructionList.push_back(load);
}
void MipsBuilder::buildCommentInst(std::string comment) {
	auto *commentInst = new MipsComment(comment);
	curBlock->instructionList.push_back(commentInst);
}
int MipsBuilder::checkStackExistence(Value *val) {
	if (value2Offset->count(val)) {
		return (*value2Offset)[val];
	} else {
		stackOffset -= 4;
		(*value2Offset)[val] = stackOffset;
		return stackOffset;
	}
}
void MipsBuilder::allocate(Value *val, MipsOperand *reg) {
	if (dynamic_cast<Constant *>(val)) {
		buildLiInst(reg, new MipsImm(((ConstantInt *)val)->value));
	} else if (val->isGlobal()) {
		string str;
		if (global2Label->count(val)) {
			str = (*global2Label)[val];
		}
		buildLiInst(reg, new MipsLabel(str));

	} else {
		int off = checkStackExistence(val);
		buildLoadInst(reg, new MipsImm(off), $sp);
	}
}
void MipsBuilder::allocateAndStore(Value *val, MipsOperand *reg) {
	if (val->isGlobal()) {
		int off = checkStackExistence(val);
		buildStoreInst(reg, new MipsImm(off), $zero);
	} else {
		int off = checkStackExistence(val);
		buildStoreInst(reg, new MipsImm(off), $sp);
	}
}
