//
// Created by hiccup on 2023/10/20.
//

#include "Function.h"

Function *Function::getint = new Function(IntegerType::INT32, "@getint");
Function *Function::putch = new Function(IntegerType::VOID, "@putch");
Function *Function::putint = new Function(IntegerType::VOID, "@putint");
Function *Function::putstr = new Function(IntegerType::VOID, "@putstr");

Function::Function(Type *ret, std::string name)
	: User(new FunctionType(), std::move(name)) {
	argumentList.clear();
	basicList.clear();
	retType = ret;
}
void Function::addArgument(Argument *arg) {
	argumentList.push_back(arg);
}
void Function::addBasicBlock(BasicBlock *bb) {
	basicList.push_back(bb);
}
Type *Function::getRetType() const {
	return retType;
}
std::string Function::toString() {
	std::stringstream ss;

	if (isLink) {
		ss << "declare " + retType->toString() + " " + this->name + "(";
		for (auto &i : argumentList) {
			ss << i->type->toString();
			if (&i!=&argumentList.back())
				ss << ", ";
		}
		ss << ")";
		return ss.str();
	}

	ss << "define dso_local " + retType->toString() + " " + this->name + "(";
	for (auto &i : argumentList) {
		ss << i->toString();
		if (&i!=&argumentList.back())
			ss << ", ";
	}
	ss << ") {\n";
	for (auto i : basicList) {
		ss << i->toString();
	}
	ss << "}";
	return ss.str();
}
