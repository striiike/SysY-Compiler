//
// Created by hiccup on 2023/9/27.
//
#pragma once

#include "ASTNode.h"
#include "TokenNode.h"

class FuncFParam : public ASTNode {
private:
	TokenNode bType;
	TokenNode ident;
	bool isArray;
	std::vector<ExpPtr> expPtrs;
public:
	FuncFParam(TokenNode bType, TokenNode ident, bool isArray, std::vector<ExpPtr> expPtrs)
		: bType(std::move(bType)),
		  ident(std::move(ident)),
		  isArray(isArray),
		  expPtrs(std::move(expPtrs)) {
		name = "<FuncFParam>";
		print();
	}

	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;

	int getDim();
	Value *llvmIr() override;
};

