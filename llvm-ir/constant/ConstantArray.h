//
// Created by hiccup on 2023/10/26.
//

#ifndef COMPILER_LLVM_IR_CONSTANT_CONSTANTARRAY_H
#define COMPILER_LLVM_IR_CONSTANT_CONSTANTARRAY_H

#include "Constant.h"
#include <vector>
using namespace std;

class ConstantArray : public Constant {
public:
	vector<int> array{};
public:
	explicit ConstantArray(vector<int> arr, int length);

	std::string toString() override;

	bool isArr() override { return true; }
};


#endif //COMPILER_LLVM_IR_CONSTANT_CONSTANTARRAY_H
