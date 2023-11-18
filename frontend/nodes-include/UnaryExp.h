//
// Created by hiccup on 2023/10/7.
//

#pragma once

#include "ASTNode.h"
#include "../lexer/Lexer.h"

class UnaryExp : public ASTNode {
	std::vector<TokenType> unaryOps;
	PrimaryExpPtr primaryExpPtr;
	FunctionCallPtr functionCallPtr;
public:
	UnaryExp(std::vector<TokenType> unaryOps, PrimaryExpPtr primaryExpPtr, FunctionCallPtr functionCallPtr);

	std::vector<TokenType> &getUnaryOps();

	PrimaryExpPtr getPrimaryExpPtr();

	FunctionCallPtr getFunctionCallPtr();

	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;

	Value *llvmIr() override;

	int evaluate();
};
