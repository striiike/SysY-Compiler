//
// Created by hiccup on 2023/9/26.
//

#pragma once

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
	IfStmt(CondPtr condPtr, StmtPtr stmtPtr, StmtPtr stmtElsePtr) :
		condPtr(condPtr), stmtPtr(stmtPtr),
		stmtElsePtr(stmtElsePtr), ComplexStmt() {}


	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	Value *llvmIr() override;
};

class _ForStmt : public ASTNode {
private:
	LValPtr lValPtr;
	ExpPtr expPtr;
public:
	_ForStmt(LValPtr lValPtr, ExpPtr expPtr)
	: lValPtr(lValPtr), expPtr(expPtr) {
		name = "<ForStmt>";
		print();
	}


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

	ForStmt(CondPtr condPtr, StmtPtr stmtPtr, _ForStmtPtr assignStmtPtr1, _ForStmtPtr assignStmtPtr2) :
		condPtr(condPtr), stmtPtr(stmtPtr),
		assignStmtPtr1(assignStmtPtr1),
		assignStmtPtr2(assignStmtPtr2), ComplexStmt() {}


	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	Value *llvmIr() override;
};
