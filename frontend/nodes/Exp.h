//
// Created by hiccup on 2023/9/22.
//

#pragma once

#include "TokenNode.h"

class Number : public ASTNode {
	int num;
public:

	Number(std::string num) :
		num(std::stoi(num)) {
		name = "<Number>";
		print();
	}

	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	int evaluate();

	Value *llvmIr() override;
};

class Exp : public ASTNode {
	bool isConst;
public:
	AddExpPtr addExpPtr;


	Exp(AddExpPtr sharedPtr, bool isConst) :
		addExpPtr(std::move(sharedPtr)), isConst(isConst) {
		name = isConst ? "<ConstExp>" : "<Exp>";
		print();
	}

	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	int evaluate();
	Value *llvmIr() override;
};

class Cond : public ASTNode {
	LOrExpPtr lOrExpPtr;
public:

	Cond(LOrExpPtr lOrExpPtr) : lOrExpPtr(std::move(lOrExpPtr)) {
		name = "<Cond>";
		print();
	}



	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	Value *llvmIr() override;
};





