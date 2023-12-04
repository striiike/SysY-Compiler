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
	std::vector<Value *> opList;
public:
	User(Type *type, std::string name);
	void addOp(Value *value);
	Value *getOp(int index);
	void updateOld2New(Value *old, Value *_new);
	void removeSelf();
};

#endif //COMPILER_LLVM_IR_USER_H
