//
// Created by hiccup on 2023/11/3.
//

#ifndef COMPILER_LLVM_IR_INSTRUCTION_ZEXTINST_H
#define COMPILER_LLVM_IR_INSTRUCTION_ZEXTINST_H

#include <utility>

#include "Instruction.h"
class ZextInst : public Instruction {
	// value that passed in
	// <res> = zext <val> to i32
public:
	ZextInst(std::string name, Value *val, Type *tar)
		: Instruction(tar, std::move(name), InstType::ZEXT) {
		addOperand(val);
	}

	std::string toString() override {
		return name + " = zext " + getOperand(0)->toLlvmString() + " to " + type->toString();
	}
};

#endif //COMPILER_LLVM_IR_INSTRUCTION_ZEXTINST_H
