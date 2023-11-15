//
// Created by hiccup on 2023/10/31.
//

#ifndef COMPILER_LLVM_IR_CONSTANT_CONSTANTSTRING_H
#define COMPILER_LLVM_IR_CONSTANT_CONSTANTSTRING_H

#include <utility>

#include "Constant.h"
#include "../Type.h"

class ConstantString : public Constant {
public:
	string str;
public:
	explicit ConstantString(string s)
		: str(std::move(s)),
		  Constant(new ArrayType((int)s.size() + 1, IntegerType::INT8), "str") {}

	static std::string llvmString(std::string s) {
		std::string target = "\n";
		std::string replacement = "\\0a";

		size_t startPos = 0;
		while ((startPos = s.find(target, startPos))!=std::string::npos) {
			s.replace(startPos, target.length(), replacement);
			startPos += replacement.length();
		}
		return s + "\\00";
	}

	static std::string mipsString(std::string s) {
		std::string target = "\n";
		std::string replacement = "\n";

		size_t startPos = 0;
		while ((startPos = s.find(target, startPos))!=std::string::npos) {
			s.replace(startPos, target.length(), replacement);
			startPos += replacement.length();
		}
		return s;
	}

	std::string toString() override {
		return type->toString() + " c\"" + llvmString(str) + "\"";
	}

	bool isStr() override { return true; }
};
#endif //COMPILER_LLVM_IR_CONSTANT_CONSTANTSTRING_H
