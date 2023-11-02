//
// Created by hiccup on 2023/10/20.
//

#ifndef COMPILER_LLVM_IR_VALUE_H
#define COMPILER_LLVM_IR_VALUE_H

#include <list>
#include <string>
#include <utility>
#include <fstream>

#include "Type.h"
class Use;
class Type;

class Value {
protected:
	Type *type;
	std::list<Use *> useList;
	std::string name;
public:
	Value(Type *type, std::string name)
		: type(type), name(std::move(name)) {}

	Type *getType() {
		return type;
	}

	std::string getName() {
		return this->name;
	}

	virtual std::string toString() {
		return " !ub! ";
	}

	std::string toLlvmString() {
		return type->toString() + " " + name;
	}
};

#endif //COMPILER_LLVM_IR_VALUE_H
