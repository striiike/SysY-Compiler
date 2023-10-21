//
// Created by hiccup on 2023/9/26.
//

#pragma once

#include <utility>
#include <vector>

#include "ASTNode.h"
#include "Stmt.h"
#include "BlockItem.h"

class ComplexStmt : public Stmt {
public:
	virtual void debug() {}
};

class IfStmt : public ComplexStmt {
private:
	CondPtr condPtr;
	StmtPtr stmtPtr;
	StmtPtr stmtElsePtr;
public :
	IfStmt(CondPtr condPtr, StmtPtr stmtPtr, StmtPtr stmtElsePtr);

	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	void llvmIr();
};

class ForStmt : public ComplexStmt {
private:
	_ForStmtPtr assignStmtPtr1;
	_ForStmtPtr assignStmtPtr2;
	CondPtr condPtr;
	StmtPtr stmtPtr;
public :
	ForStmt(CondPtr condPtr, StmtPtr stmtPtr,
			_ForStmtPtr assignStmtPtr1, _ForStmtPtr assignStmtPtr2);

	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	void llvmIr();
};

class Block : public ComplexStmt {
private:
	std::vector<BlockItemPtr> blockItemPtrs;
	TokenNode rbrace;
public:
	explicit Block(std::vector<BlockItemPtr> blockItemPtrs, TokenNode rbrace);

	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	void llvmIr();
};