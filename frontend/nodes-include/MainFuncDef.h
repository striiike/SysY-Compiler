//
// Created by hiccup on 2023/9/27.
//
#pragma once

#include "ASTNode.h"
#include "TokenNode.h"

class MainFuncDef : public ASTNode {
private:
	TokenNode bType;
	TokenNode _main;
	BlockPtr blockPtr;
public:
	MainFuncDef(TokenNode bType, TokenNode _main, BlockPtr blockPtr);

	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	Value *llvmIr() override;
};

