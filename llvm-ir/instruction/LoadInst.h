//
// Created by hiccup on 2023/10/29.
//

#ifndef COMPILER_LLVM_IR_INSTRUCTION_LOADINST_H
#define COMPILER_LLVM_IR_INSTRUCTION_LOADINST_H

#include <utility>

#include "Instruction.h"
class LoadInst : public Instruction {
public:
	LoadInst(std::string name, Value *addr)
		: Instruction(addr->getType()->getTargetType(), std::move(name), InstType::LOAD) {
		addOperand(addr);
	}

	std::string toString() override {
		return name + " = load " + type->toString() + ", " +
			getOperand(0)->getType()->toString() + " " + getOperand(0)->getName();
	}
};

#endif //COMPILER_LLVM_IR_INSTRUCTION_LOADINST_H
