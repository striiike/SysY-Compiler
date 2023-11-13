//
// Created by hiccup on 2023/9/27.
//

#ifndef COMPILER_FUNCDEF_H
#define COMPILER_FUNCDEF_H

#include <utility>

#include "TokenNode.h"
#include "FuncFParams.h"
#include "ComplexStmt.h"
#include "Block.h"

class FuncDef : public ASTNode {
private:
	TokenNode funcType;
	TokenNode ident;
	FuncFParamsPtr funcFParamsPtr;
	BlockPtr blockPtr;
public:
	FuncDef(TokenNode funcType, TokenNode ident, FuncFParamsPtr funcFParamsPtr, BlockPtr blockPtr);

	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	Value *llvmIr() override;
};

#endif //COMPILER_FUNCDEF_H

