//
// Created by hiccup on 2023/10/20.
//

#ifndef COMPILER_LLVM_IR_GLOBALVARIABLE_H
#define COMPILER_LLVM_IR_GLOBALVARIABLE_H

#include <utility>

#include "User.h"
#include "constant/Constant.h"

class GlobalVariable : public User {
	Constant *init;
	bool isConst;
public:
	GlobalVariable(Type *ty, std::string name, bool isConst, Constant *init = nullptr)
		: User(ty, std::move(name)), init(init), isConst(isConst) {}

	// init exists under guarantee
	std::string toString() override {
		return name + " = dso_local " + ((isConst) ? "constant" : "global") + " " + init->toString();
	}

};

#endif //COMPILER_LLVM_IR_GLOBALVARIABLE_H
