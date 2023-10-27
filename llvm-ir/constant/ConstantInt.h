//
// Created by hiccup on 2023/10/26.
//

#ifndef COMPILER_LLVM_IR_CONSTANT_CONSTANTINT_H
#define COMPILER_LLVM_IR_CONSTANT_CONSTANTINT_H

#include "Constant.h"

class ConstantInt : public Constant {
	int value;
	int bitWidth; // for 1 or 32
public:
	explicit ConstantInt(int value, int bitWidth = 32)
		: value(value), bitWidth(bitWidth),
		  Constant(new IntegerType(bitWidth), to_string(value)) {}

	std::string toString() override {
		return type->toString() + " " + std::to_string(value);
	}
};

#endif //COMPILER_LLVM_IR_CONSTANT_CONSTANTINT_H
