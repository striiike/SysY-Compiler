//
// Created by hiccup on 2023/10/20.
//

#ifndef COMPILER_LLVM_IR_GLOBALVARIABLE_H
#define COMPILER_LLVM_IR_GLOBALVARIABLE_H

#include <utility>

#include "User.h"
#include "Initializer.h"

class GlobalVariable : public User {
	Initializer *init;
public:
	GlobalVariable(Type *ty, std::string name, Initializer *init = nullptr)
		: User(ty, std::move(name)), init(init) {}

};

#endif //COMPILER_LLVM_IR_GLOBALVARIABLE_H
