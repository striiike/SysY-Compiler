//
// Created by hiccup on 2023/9/22.
//

#pragma once

#include "TokenNode.h"

class Number : public ASTNode {
	int num;
public:
	explicit Number(std::string num);

	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	int evaluate();

	Value *llvmIr() override;
};

class Exp : public ASTNode {
	bool isConst;
public:
	AddExpPtr addExpPtr;

	explicit Exp(AddExpPtr sharedPtr, bool isConst);

	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	int evaluate();
	Value *llvmIr() override;
};

class Cond : public ASTNode {
	LOrExpPtr lOrExpPtr;
public:
	explicit Cond(LOrExpPtr lOrExpPtr);

	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	Value *llvmIr() override;
};





