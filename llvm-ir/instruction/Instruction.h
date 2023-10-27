//
// Created by hiccup on 2023/10/20.
//

#ifndef COMPILER_LLVM_IR_INSTRUCTION_H
#define COMPILER_LLVM_IR_INSTRUCTION_H

#include <utility>

#include "../BasicBlock.h"
#include "../User.h"

class BasicBlock;

enum InstType {
	ALLOCA, RETURN, STORE, GEP, ALU
};

class Instruction : public User {
	BasicBlock *parent{};
	InstType instType;
public:

	/// @name for return reg, @ty for inst type, @parent for bb, @params for operands

	Instruction(Type *ty, std::string name, InstType instTy)
		: User(ty, std::move(name)), instType(instTy) {
	}
};

#endif //COMPILER_LLVM_IR_INSTRUCTION_H
