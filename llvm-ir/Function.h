//
// Created by hiccup on 2023/10/20.
//

#ifndef COMPILER_LLVM_IR_FUNCTION_H
#define COMPILER_LLVM_IR_FUNCTION_H

#include "BasicBlock.h"
#include <string>
#include <utility>

class Function;

class Argument : public Value {
	Function *parent{};
public:
	Argument(Type *type, std::string name)
		: Value(type, std::move(name)) {}

	void setFunction(Function *PFunction) {
		this->parent = PFunction;
	}
};

class Function : public User {
	std::vector<Argument *> argumentList;
	std::list<BasicBlock *> basicList;
	Type *retType;
public:
	Function(Type *ret, std::string name)
		: User(new FunctionType(), std::move(name)) {
		argumentList.clear();
		basicList.clear();
		retType = ret;
	}

	void addArgument(Argument *arg) {
		argumentList.push_back(arg);
	}

	void addBasicBlock(BasicBlock *bb) {
		basicList.push_back(bb);
	}
};

#endif //COMPILER_LLVM_IR_FUNCTION_H
