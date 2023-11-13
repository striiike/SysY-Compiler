//
// Created by hiccup on 2023/10/26.
//

#ifndef COMPILER_LLVM_IR_CONSTANT_CONSTANTARRAY_H
#define COMPILER_LLVM_IR_CONSTANT_CONSTANTARRAY_H

#include "Constant.h"
#include <utility>
#include <vector>
using namespace std;

class ConstantArray : public Constant {
	vector<int> array{};
public:
	explicit ConstantArray(vector<int> arr, int length)
		: array(std::move(arr)),
		  Constant(new ArrayType(length, IntegerType::INT32),
				   "constant") {}

	std::string toString() override {
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
};

#endif //COMPILER_LLVM_IR_CONSTANT_CONSTANTARRAY_H
