//
// Created by hiccup on 2023/10/9.
//

#pragma once

#include "ASTNode.h"
#include "TokenNode.h"

class FuncRParams : public ASTNode {
	std::vector<ExpPtr> expPtrs;
public:
	explicit FuncRParams(std::vector<ExpPtr> expPtrs);

	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	vector<Value *> llvmIrList();
};

class FunctionCall : public ASTNode {
	TokenNode ident;
	FuncRParamsPtr funcRParamsPtr;
public:
	FunctionCall(TokenNode ident, FuncRParamsPtr funcRParamsPtr);

	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	Value *llvmIr() override;
};

