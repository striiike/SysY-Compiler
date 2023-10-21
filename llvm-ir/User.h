//
// Created by hiccup on 2023/10/20.
//

#ifndef COMPILER_LLVM_IR_USER_H
#define COMPILER_LLVM_IR_USER_H

#include "Value.h"
#include <utility>
#include <vector>

class User : public Value {
	std::vector<Value *> operandList;
public:
	User(Type *type, std::string name)
		: Value(type, std::move(name)) {
		operandList.clear();
	}

	void addOperand(Value *value) { operandList.push_back(value); }

};

#endif //COMPILER_LLVM_IR_USER_H
