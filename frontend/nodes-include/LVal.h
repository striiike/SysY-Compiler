//
// Created by hiccup on 2023/10/9.
//
#pragma once

#include "ASTNode.h"
#include "TokenNode.h"

class LVal : public ASTNode {
	TokenNode ident;
	std::vector<ExpPtr> array;
public:
	LVal(TokenNode ident, std::vector<ExpPtr> array) : ident(std::move(ident)), array(std::move(array)) {
		name = "<LVal>";
		print();
	}


	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	int evaluate();
	Value *llvmIr() override;
	Value *llvmIrAddr();
};

