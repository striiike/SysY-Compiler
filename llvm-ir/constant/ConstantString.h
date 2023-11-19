//
// Created by hiccup on 2023/10/31.
//

#ifndef COMPILER_LLVM_IR_CONSTANT_CONSTANTSTRING_H
#define COMPILER_LLVM_IR_CONSTANT_CONSTANTSTRING_H

#include "Constant.h"
#include "../Type.h"

class ConstantString : public Constant {
public:
	string str;
public:
	explicit ConstantString(string s);

	static std::string llvmString(std::string s);

	std::string mipsString();

	std::string toString() override;

	bool isStr() override { return true; }
};
#endif //COMPILER_LLVM_IR_CONSTANT_CONSTANTSTRING_H
