//
// Created by hiccup on 2023/9/26.
//

#pragma once

#include <utility>

#include "ASTNode.h"
#include "Decl.h"
#include "Stmt.h"

class BlockItem : public ASTNode {
private:
	DeclPtr declPtr;
	StmtPtr stmtPtr;
public:
	BlockItem(DeclPtr declPtr, StmtPtr stmtPtr);

	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;

	Value *llvmIr() override;
};



