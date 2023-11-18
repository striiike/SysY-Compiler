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
	AssignStmt(LValPtr lValPtr, ExpPtr expPtr);

	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	Value *llvmIr() override;
};

class ExpStmt : public SimpleStmt {
private:
	ExpPtr expPtr;
public:
	explicit ExpStmt(ExpPtr expPtr);

	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	Value *llvmIr() override;
};

class BreakStmt : public SimpleStmt {
private:
	TokenNode _break;
public:
	explicit BreakStmt(TokenNode _break);

	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	Value *llvmIr() override;
};

class ContinueStmt : public SimpleStmt {
private:
	TokenNode _continue;
public:
	explicit ContinueStmt(TokenNode _continue);

	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	Value *llvmIr() override;
};

class ReturnStmt : public SimpleStmt {
private:
	TokenNode _return;
	ExpPtr expPtr;
public:
	ReturnStmt(TokenNode _return, ExpPtr expPtr);

	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	Value *llvmIr() override;
};

class GetintStmt : public SimpleStmt {
private:
	LValPtr lValPtr;
	TokenNode _getint;
public:
	GetintStmt(LValPtr lValPtr, TokenNode _getint);

	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	Value *llvmIr() override;
};

class PrintfStmt : public SimpleStmt {
private:
	TokenNode _printf;
	TokenNode formatString;
	std::vector<ExpPtr> expPtrs;
public:
	PrintfStmt(TokenNode _printf, TokenNode FormatString, std::vector<ExpPtr> expPtrs);

	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	Value *llvmIr() override;
};
