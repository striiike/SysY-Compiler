//
// Created by hiccup on 2023/10/20.
//

#ifndef COMPILER_LLVM_IR_MODULE_H
#define COMPILER_LLVM_IR_MODULE_H

#include <list>
#include <vector>
#include "GlobalVariable.h"
#include "Function.h"

class Module {
public:
	std::vector<GlobalVariable *> globalList;
	std::list<Function *> functionList;

public:
	Module();;

	void addGlobalVariable(GlobalVariable *globalVar);

	void addFunction(Function *func);

	std::string toString();
};

#endif //COMPILER_LLVM_IR_MODULE_H
