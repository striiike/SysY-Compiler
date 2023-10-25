//
// Created by hiccup on 2023/10/20.
//

#ifndef COMPILER_LLVM_IR_USE_H
#define COMPILER_LLVM_IR_USE_H

#include "User.h"
#include "Value.h"

class User;
class Value;
class Use {
	User *user;
	Value *value;
public:
	Use(User *user, Value *value)
		: user(user), value(value) {}

};

#endif //COMPILER_LLVM_IR_USE_H
