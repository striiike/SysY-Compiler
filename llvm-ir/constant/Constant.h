//
// Created by hiccup on 2023/10/20.
//

#ifndef COMPILER_LLVM_IR_CONSTANT_H
#define COMPILER_LLVM_IR_CONSTANT_H

#include "../Value.h"
#include <string>
#include <utility>
using namespace std;

class Constant : public Value {
public:
	Constant(Type *ty, string name) : Value(ty, std::move(name)) {}

	Type *getType() {
		return this->type;
	}

	std::string toString() override { return "?"; }

};

#endif //COMPILER_LLVM_IR_CONSTANT_H
