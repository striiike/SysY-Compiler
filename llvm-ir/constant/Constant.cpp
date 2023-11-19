//
// Created by hiccup on 2023/11/18.
//
#include "ConstantInt.h"
#include "ConstantArray.h"
#include "ConstantString.h"
#include <sstream>

ConstantArray::ConstantArray(vector<int> arr, int length)
	: array(std::move(arr)),
	  Constant(new ArrayType(length, IntegerType::INT32),
			   "constant") {}
std::string ConstantArray::toString() {
	if (array.empty()) {
		return type->toString() + " zeroinitializer";
	} else {
		std::stringstream ss;
		for (int &value : array) {
			ss << "i32 " << value;
			if (&value!=&array.back()) {
				ss << ", ";
			}
		}
		return type->toString() + " [" + ss.str() + "]";
	}
}


ConstantInt::ConstantInt(int value, int bitWidth)
	: value(value), bitWidth(bitWidth),
	  Constant(IntegerType::INT32, to_string(value)) {}

std::string ConstantInt::toString() {
	if (type->isVoid())
		return "void";
	return type->toString() + " " + std::to_string(value);
}



ConstantString::ConstantString(string s)
	: str(std::move(s)),
	  Constant(new ArrayType((int)s.size() + 1, IntegerType::INT8), "str") {}

std::string ConstantString::llvmString(std::string s) {
	std::string target = "\n";
	std::string replacement = "\\0a";

	size_t startPos = 0;
	while ((startPos = s.find(target, startPos))!=std::string::npos) {
		s.replace(startPos, target.length(), replacement);
		startPos += replacement.length();
	}
	return s + "\\00";
}

std::string ConstantString::mipsString() {
	std::string target = "\n";
	std::string replacement = "\\n";

	size_t startPos = 0;
	while ((startPos = str.find(target, startPos))!=std::string::npos) {
		str.replace(startPos, target.length(), replacement);
		startPos += replacement.length();
	}
	return str;
}

std::string ConstantString::toString() {
	return type->toString() + " c\"" + llvmString(str) + "\"";
}