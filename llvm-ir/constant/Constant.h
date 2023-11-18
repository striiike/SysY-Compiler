//
// Created by hiccup on 2023/10/20.
//

#ifndef COMPILER_LLVM_IR_CONSTANT_H
#define COMPILER_LLVM_IR_CONSTANT_H

#include "../Value.h"
#include <string>
using namespace std;

class Constant : public Value {
public:
	Constant(Type *ty, string name) : Value(ty, std::move(name)) {}

	Type *getType() {
		return this->type;
	}

	std::string toString() override { return "?"; }

	virtual bool isInt() { return false; }
	virtual bool isArr() { return false; }
	virtual bool isStr() { return false; }

};

#endif //COMPILER_LLVM_IR_CONSTANT_H
