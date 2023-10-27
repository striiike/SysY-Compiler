//
// Created by hiccup on 2023/10/20.
//

#ifndef COMPILER_LLVM_IR_GLOBALVARIABLE_H
#define COMPILER_LLVM_IR_GLOBALVARIABLE_H

#include <utility>

#include "User.h"
#include "Initializer.h"
#include "constant/Constant.h"

class GlobalVariable : public User {
	Constant *init;
public:
	GlobalVariable(Type *ty, std::string name, Constant *init = nullptr)
		: User(ty, std::move(name)), init(init) {}

	// init exists under guarantee
	std::string toString() {
		return name + " = dso_local global " + init->toString();
	}

};

#endif //COMPILER_LLVM_IR_GLOBALVARIABLE_H
