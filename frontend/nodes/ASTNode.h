//
// Created by hiccup on 2023/9/21.
//

#pragma once

#include <iostream>
#include "../../config.h"
#include "../parser/Exception.h"
#include <memory>
#include <string>
#include <fstream>

class Value;

class ASTNode {
public:
	std::string name;

	void print() const {
		if (PARSER_DISPLAY && PARSER_SWITCH)
			outfile << name << std::endl;
	}

	virtual void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	}

	virtual Value *llvmIr() {
		return nullptr;
	}
};

class MulExp;
class AddExp;
class RelExp;
class EqExp;
class LAndExp;
class LOrExp;
class Exp;
class LVal;
class PrimaryExp;
class Number;
class FunctionCall;
class FuncRParams;
class UnaryExp;
class ConstExp;
class Cond;

using UnaryExpPtr = UnaryExp *;
using MulExpPtr = MulExp *;
using AddExpPtr = AddExp *;
using RelExpPtr = RelExp *;
using EqExpPtr = EqExp *;
using LAndExpPtr = LAndExp *;
using LOrExpPtr = LOrExp *;

using ExpPtr = Exp *;
using LValPtr = LVal *;
using PrimaryExpPtr = PrimaryExp *;
using NumberPtr = Number *;
using FunctionCallPtr = FunctionCall *;
using FuncRParamsPtr = FuncRParams *;
using UnaryExpPtr = UnaryExp *;
using CondPtr = Cond *;

class Def;
class InitVal;
class ArrayInitVal;
class ExpInitVal;
class Decl;

using DefPtr = Def *;
using InitValPtr = InitVal *;
using ArrayInitValPtr = ArrayInitVal *;
using ExpInitValPtr = ExpInitVal *;
using DeclPtr = Decl *;

class Stmt;
class SimpleStmt;
class ComplexStmt;
class BlockItem;
class Block;
class AssignStmt;
class ExpStmt;
class BreakStmt;
class ContinueStmt;
class ReturnStmt;
class GetintStmt;
class PrintfStmt;
class IfStmt;
class _ForStmt;
class ForStmt;

using StmtPtr = Stmt *;
using SimpleStmtPtr = SimpleStmt *;
using ComplexStmtPtr = ComplexStmt *;
using BlockItemPtr = BlockItem *;
using BlockPtr = Block *;
using AssignStmtPtr = AssignStmt *;
using ExpStmtPtr = ExpStmt *;
using BreakStmtPtr = BreakStmt *;
using ContinueStmtPtr = ContinueStmt *;
using ReturnStmtPtr = ReturnStmt *;
using GetintStmtPtr = GetintStmt *;
using PrintfStmtPtr = PrintfStmt *;
using IfStmtPtr = IfStmt *;
using ForStmtPtr = ForStmt *;
using _ForStmtPtr = _ForStmt *;

class FuncDef;
class FuncFParams;
class FuncFParam;
class MainFuncDef;

using FuncDefPtr = FuncDef *;
using FuncFParamsPtr = FuncFParams *;
using FuncFParamPtr = FuncFParam *;
using MainFuncDefPtr = MainFuncDef *;

class CompUnit;
