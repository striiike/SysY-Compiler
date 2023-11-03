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
	Value *llvmIr() override;
};


class _ForStmt : public ASTNode {
private:
	LValPtr lValPtr;
	ExpPtr expPtr;
public:
	_ForStmt(LValPtr lValPtr, ExpPtr expPtr);

	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	Value *llvmIr() override;
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
	Value *llvmIr() override;
};
