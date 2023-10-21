//
// Created by hiccup on 2023/10/20.
//

#ifndef COMPILER_LLVM_IR_INITIALIZER_H
#define COMPILER_LLVM_IR_INITIALIZER_H

#include <utility>
#include <vector>
#include "Type.h"

class Initializer {
	std::vector<int> array;
	Type *type;
public:
	Initializer(std::vector<int> arr, Type *ty)
		: array(std::move(arr)), type(ty) {}

};

#endif //COMPILER_LLVM_IR_INITIALIZER_H
