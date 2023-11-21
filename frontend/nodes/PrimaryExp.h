//
// Created by hiccup on 2023/10/7.
//

#pragma once
#include "ASTNode.h"

class PrimaryExp : public ASTNode {
public:
	ExpPtr expPtr = nullptr;
	LValPtr lValPtr = nullptr;
	NumberPtr numberPtr = nullptr;
public:
	PrimaryExp(ExpPtr exp, LValPtr lVal, NumberPtr number)
		: expPtr(std::move(exp)), lValPtr(std::move(lVal)),
		  numberPtr(std::move(number)) {
		name = "<PrimaryExp>";
		print();
	}

	// Getter methods remain unchanged

	LValPtr getLValPtr() { return lValPtr; }

	NumberPtr getNumberPtr() { return numberPtr; }

	ExpPtr getExpPtr() { return expPtr; }


	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	int evaluate();

	Value *llvmIr() override;
};
