//
// Created by hiccup on 2023/10/7.
//

#pragma once
#include "ASTNode.h"

class PrimaryExp : public ASTNode {
	ExpPtr expPtr = nullptr;
	LValPtr lValPtr = nullptr;
	NumberPtr numberPtr = nullptr;
public:
	PrimaryExp(ExpPtr exp, LValPtr lVal, NumberPtr number);

	// Getter methods remain unchanged
	ExpPtr getExpPtr();

	LValPtr getLValPtr();

	NumberPtr getNumberPtr();

	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	int evaluate();

	Value *llvmIr() override;
};
