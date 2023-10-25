//
// Created by hiccup on 2023/10/20.
//

#ifndef COMPILER_LLVM_IR_VALUE_H
#define COMPILER_LLVM_IR_VALUE_H

#include "Use.h"
#include "Type.h"
#include <list>
#include <string>
#include <utility>
#include <fstream>

class Value {
protected:
	Type *type;
	std::list<Use *> useList;
	std::string name;
public:
	Value(Type *type, std::string name)
		: type(type), name(std::move(name)) {}
};

#endif //COMPILER_LLVM_IR_VALUE_H
