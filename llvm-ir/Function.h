//
// Created by hiccup on 2023/10/20.
//

#ifndef COMPILER_LLVM_IR_FUNCTION_H
#define COMPILER_LLVM_IR_FUNCTION_H

#include "BasicBlock.h"
#include "Type.h"
#include <string>
#include <utility>
#include <sstream>
#include <unordered_map>

class Function;

class Argument : public Value {
public:
	Function *parent{};
	int rank{};
public:
	Argument(Type *type, std::string name)
		: Value(type, std::move(name)) {}

	void setFunction(Function *PFunction) {
		this->parent = PFunction;
	}

	std::string toString() override {
		return type->toString() + " " + name;
	}
};

class Function : public User {
public:
	std::vector<Argument *> argumentList;
	std::list<BasicBlock *> basicList;
	Type *retType;

	// CFG
	unordered_map<BasicBlock *, vector<BasicBlock *> *>
		*preMap = new unordered_map<BasicBlock *, vector<BasicBlock *> *>();
	unordered_map<BasicBlock *, vector<BasicBlock *> *>
		*sucMap = new unordered_map<BasicBlock *, vector<BasicBlock *> *>();

	// dominateTree
	unordered_map<BasicBlock *, vector<BasicBlock *> *>
		*dominateTree = new unordered_map<BasicBlock *, vector<BasicBlock *> *>();

public:
	static Function *getint;
	static Function *putch;
	static Function *putint;
	static Function *putstr;

	bool isLink = false;

	Function(Type *ret, std::string name)
		: User(new FunctionType(), std::move(name)) {
		argumentList.clear();
		basicList.clear();
		retType = ret;
	}

	void addArgument(Argument *arg) {
		argumentList.push_back(arg);
		arg->rank = (int)argumentList.size();
	}

	void addBasicBlock(BasicBlock *bb) {
		basicList.push_back(bb);
	}

	Type *getRetType() {
		return retType;
	}

	std::string toString() override {
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
};

#endif //COMPILER_LLVM_IR_FUNCTION_H
