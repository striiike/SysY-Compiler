//
// Created by hiccup on 2023/9/26.
//

#pragma once

#include "TokenNode.h"
#include "ASTNode.h"
#include "Stmt.h"

class SimpleStmt : public Stmt {
public:
	virtual void debug() {}
};

class AssignStmt : public SimpleStmt {
private:
	LValPtr lValPtr;
	ExpPtr expPtr;
public:
	AssignStmt(LValPtr lValPtr, ExpPtr expPtr)
		: lValPtr(lValPtr), expPtr(expPtr), SimpleStmt() {}


	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	Value *llvmIr() override;
};

class ExpStmt : public SimpleStmt {
private:
	ExpPtr expPtr;
public:

	ExpStmt(ExpPtr expPtr)
		: expPtr(expPtr), SimpleStmt() {}





	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	Value *llvmIr() override;
};

class BreakStmt : public SimpleStmt {
private:
	TokenNode _break;
public:

	BreakStmt(TokenNode _break)
		: _break(std::move(_break)), SimpleStmt() {}




	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	Value *llvmIr() override;
};

class ContinueStmt : public SimpleStmt {
private:
	TokenNode _continue;
public:


	ContinueStmt(TokenNode _continue)
		: _continue(std::move(_continue)), SimpleStmt() {}




	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	Value *llvmIr() override;
};

class ReturnStmt : public SimpleStmt {
private:
	TokenNode _return;
	ExpPtr expPtr;
public:


	ReturnStmt(TokenNode _return, ExpPtr expPtr)
		: _return(std::move(_return)), expPtr(expPtr), SimpleStmt() {}



	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	Value *llvmIr() override;
};

class GetintStmt : public SimpleStmt {
private:
	LValPtr lValPtr;
	TokenNode _getint;
public:


	GetintStmt(LValPtr lValPtr, TokenNode _getint)
		: lValPtr(std::move(lValPtr)), _getint(std::move(_getint)), SimpleStmt() {}



	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	Value *llvmIr() override;
};

class PrintfStmt : public SimpleStmt {
private:
	TokenNode _printf;
	TokenNode formatString;
	std::vector<ExpPtr> expPtrs;
public:
	PrintfStmt(TokenNode _printf, TokenNode FormatString, std::vector<ExpPtr> expPtrs)
		: _printf(std::move(_printf)), formatString(std::move(FormatString)), expPtrs(std::move(expPtrs)),
		  SimpleStmt() {}

	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	Value *llvmIr() override;
};
