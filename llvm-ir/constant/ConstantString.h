//
// Created by hiccup on 2023/10/31.
//

#ifndef COMPILER_LLVM_IR_CONSTANT_CONSTANTSTRING_H
#define COMPILER_LLVM_IR_CONSTANT_CONSTANTSTRING_H

#include <utility>

#include "Constant.h"
#include "../Type.h"

class ConstantString : public Constant {
	string str;
public:
	explicit ConstantString(string s)
		: str(std::move(s)),
		  Constant(new ArrayType((int)s.size(), IntegerType::INT8), "str") {}

	std::string toString() override {
		return type->toString() + " c\"" + str + "\"";
	}
};
#endif //COMPILER_LLVM_IR_CONSTANT_CONSTANTSTRING_H
