//
// Created by hiccup on 2023/10/20.
//

#ifndef COMPILER_LLVM_IR_USER_H
#define COMPILER_LLVM_IR_USER_H

#include "Value.h"
#include <utility>
#include <vector>
#include <cassert>

#include "Use.h"

class User : public Value {
public:
	std::vector<Value *> operandList;
public:
	User(Type *type, std::string name)
		: Value(type, std::move(name)) {
		operandList.clear();
	}

	void addOperand(Value *value) {
		assert(value!=nullptr);
		operandList.push_back(value);
		value->useList.push_back(new Use(this, value));
	}

	Value *getOperand(int index) {
		if (index >= operandList.size()) return nullptr;
		return operandList[index];
	}

	void updateOld2New(Value *old, Value *_new);


};

#endif //COMPILER_LLVM_IR_USER_H
