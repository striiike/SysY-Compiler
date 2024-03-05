//
// Created by hiccup on 2023/10/20.
//

#ifndef COMPILER_LLVM_IR_INSTRUCTION_H
#define COMPILER_LLVM_IR_INSTRUCTION_H

#include <utility>

#include "../User.h"
#include "../Type.h"

class BasicBlock;

enum InstType {
	ALLOCA, RETURN, STORE, GEP, ALU, LOAD, CALL, ICMP, BRANCH, ZEXT, PHI, PC, MOVE
};

class Instruction : public User {
public:
	BasicBlock *parent{};
public:
	InstType instType;
	/// @name for return reg, @ty for inst type, @parent for bb, @params for operands

	Instruction(Type *ty, std::string name, InstType instTy)
		: User(ty, std::move(name)), instType(instTy) {
	}

	void removeUse() {
		for (auto i : opList) {
			i->useList.remove_if([this](Use *u) { return (u->user)==this; });
		}
	}
};

#endif //COMPILER_LLVM_IR_INSTRUCTION_H
