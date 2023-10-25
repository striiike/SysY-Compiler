//
// Created by hiccup on 2023/10/20.
//

#ifndef COMPILER_LLVM_IR_INITIALIZER_H
#define COMPILER_LLVM_IR_INITIALIZER_H

#include <utility>
#include <vector>
#include "Type.h"
#include <sstream>

class Initializer {
	std::vector<int> array;
	Type *type;
public:
	Initializer(Type *ty, std::vector<int> arr)
		: array(std::move(arr)), type(ty) {}

	std::string toString() {
		if (array.empty()) {
			if (type->isArray())
				return type->toString() + " zeroinitializer";
			else
				return type->toString() + " 0";
		} else {
			if (type->isArray()) {
				std::stringstream ss;
				for (int &value : array) {
					ss << "i32 " << value;
					if (&value != &array.back()) {
						ss << ", ";
					}
				}
				return type->toString() + " [" + ss.str() + "]";
			} else {
				return type->toString() + " " + std::to_string(array[0]);
			}
		}
	}
};

#endif //COMPILER_LLVM_IR_INITIALIZER_H
