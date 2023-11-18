//
// Created by hiccup on 2023/10/22.
//

#ifndef COMPILER_LLVM_IR_INSTRUCTION_RETURNINST_H
#define COMPILER_LLVM_IR_INSTRUCTION_RETURNINST_H

#include <utility>

#include "Instruction.h"

class ReturnInst : public Instruction {
public:
	ReturnInst(std::string name, Value *ret)
		: Instruction(ret->type, std::move(name), InstType::RETURN) {
		addOperand(ret);
	}

	std::string toString() override {
		return "ret " + getOperand(0)->toLlvmString();
	}
};

#endif //COMPILER_LLVM_IR_INSTRUCTION_RETURNINST_H
