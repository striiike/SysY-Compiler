//
// Created by hiccup on 2023/10/9.
//

#pragma once

#include "ASTNode.h"
#include "TokenNode.h"

class FuncRParams : public ASTNode {
	std::vector<ExpPtr> expPtrs;
public:
	FuncRParams(std::vector<ExpPtr> expPtrs) : expPtrs(std::move(expPtrs)) {
		name = "<FuncRParams>";
		print();
	}



	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	vector<Value *> llvmIrList();
};

class FunctionCall : public ASTNode {
	TokenNode ident;
	FuncRParamsPtr funcRParamsPtr;
public:
	FunctionCall(TokenNode ident, FuncRParamsPtr funcRParamsPtr)
		: ident(std::move(ident)), funcRParamsPtr(std::move(funcRParamsPtr)) {
		name = "<FunctionCall>";
	}


	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	Value *llvmIr() override;
};

