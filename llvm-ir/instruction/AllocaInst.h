//
// Created by hiccup on 2023/10/26.
//

#ifndef COMPILER_LLVM_IR_INSTRUCTION_ALLOCAINST_H
#define COMPILER_LLVM_IR_INSTRUCTION_ALLOCAINST_H

#include <utility>

#include "Instruction.h"

class AllocaInst : public Instruction {
	Type *retType;
public:
	// <result> = alloca <type>
	AllocaInst(Type *ty, std::string name)
		: retType(ty),
		  Instruction(new PointerType(ty), std::move(name), InstType::ALLOCA) {}

	std::string toString() override {
		return name + " = alloca " + type->toString();
	}
};

#endif //COMPILER_LLVM_IR_INSTRUCTION_ALLOCAINST_H
