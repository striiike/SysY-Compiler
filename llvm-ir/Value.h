//
// Created by hiccup on 2023/10/20.
//

#ifndef COMPILER_LLVM_IR_VALUE_H
#define COMPILER_LLVM_IR_VALUE_H

#include <list>
#include <string>
#include "Type.h"


class Use;
class Type;

class Value {
public:
	Type *type;
	/// here use means @used_by other users
	std::list<Use *> useList;
	std::string name;
public:
	Value(Type *type, std::string name)
		: type(type), name(std::move(name)) {}



	virtual std::string toString() {
		return " !ub! ";
	}

	std::string toLlvmString() {
		return type->toString() + " " + name;
	}

	/// modify all users that use @this value to @new value
	void replaceOld2New(Value *_new);

	virtual bool isGlobal() {return false;}

};



#endif //COMPILER_LLVM_IR_VALUE_H
