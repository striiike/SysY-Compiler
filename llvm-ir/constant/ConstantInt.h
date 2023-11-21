//
// Created by hiccup on 2023/10/26.
//

#ifndef COMPILER_LLVM_IR_CONSTANT_CONSTANTINT_H
#define COMPILER_LLVM_IR_CONSTANT_CONSTANTINT_H

#include "Constant.h"

class ConstantInt : public Constant {
public:
	int value;
	int bitWidth; // for 1 or 32
public:
	explicit ConstantInt(int value, int bitWidth = 32)
		: value(value), bitWidth(bitWidth),
		  Constant(IntegerType::INT32, to_string(value)) {}

	std::string toString() override {
		if (type->isVoid())
			return "void";
		return type->toString() + " " + std::to_string(value);
	}

	bool isInt() override { return true; }
};

#endif //COMPILER_LLVM_IR_CONSTANT_CONSTANTINT_H
