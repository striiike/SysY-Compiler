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
	std::vector<GlobalVariable *> globalList;
	std::list<Function *> functionList;

public:
	Module() = default;

	void addGlobalVariable(GlobalVariable *globalVar) {
		globalList.push_back(globalVar);
	}

	void addFunction(Function *func) {
		functionList.push_back(func);
	}

	std::string toString() {
		std::string str;
		for (auto i : globalList) {
			str += i->toString();
			str += "\n";
		}
		for (auto i : functionList) {
			str += i->toString();
			str += "\n";
		}
		return str;
	}
};

#endif //COMPILER_LLVM_IR_MODULE_H
