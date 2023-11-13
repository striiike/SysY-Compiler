//
// Created by hiccup on 2023/9/21.
//

#pragma once

#include <iostream>
#include "../../config.h"
#include "../parser/Exception.hpp"
#include "../../llvm-ir/IrBuilder.h"
#include <memory>
#include <string>
#include <fstream>

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

using UnaryExpPtr = std::shared_ptr<UnaryExp>;
using MulExpPtr = std::shared_ptr<MulExp>;
using AddExpPtr = std::shared_ptr<AddExp>;
using RelExpPtr = std::shared_ptr<RelExp>;
using EqExpPtr = std::shared_ptr<EqExp>;
using LAndExpPtr = std::shared_ptr<LAndExp>;
using LOrExpPtr = std::shared_ptr<LOrExp>;

using ExpPtr = std::shared_ptr<Exp>;
using LValPtr = std::shared_ptr<LVal>;
using PrimaryExpPtr = std::shared_ptr<PrimaryExp>;
using NumberPtr = std::shared_ptr<Number>;
using FunctionCallPtr = std::shared_ptr<FunctionCall>;
using FuncRParamsPtr = std::shared_ptr<FuncRParams>;
using UnaryExpPtr = std::shared_ptr<UnaryExp>;
using CondPtr = std::shared_ptr<Cond>;

class Def;
class InitVal;
class ArrayInitVal;
class ExpInitVal;
class Decl;

using DefPtr = std::shared_ptr<Def>;
using InitValPtr = std::shared_ptr<InitVal>;
using ArrayInitValPtr = std::shared_ptr<ArrayInitVal>;
using ExpInitValPtr = std::shared_ptr<ExpInitVal>;
using DeclPtr = std::shared_ptr<Decl>;

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

using StmtPtr = std::shared_ptr<Stmt>;
using SimpleStmtPtr = std::shared_ptr<SimpleStmt>;
using ComplexStmtPtr = std::shared_ptr<ComplexStmt>;
using BlockItemPtr = std::shared_ptr<BlockItem>;
using BlockPtr = std::shared_ptr<Block>;
using AssignStmtPtr = std::shared_ptr<AssignStmt>;
using ExpStmtPtr = std::shared_ptr<ExpStmt>;
using BreakStmtPtr = std::shared_ptr<BreakStmt>;
using ContinueStmtPtr = std::shared_ptr<ContinueStmt>;
using ReturnStmtPtr = std::shared_ptr<ReturnStmt>;
using GetintStmtPtr = std::shared_ptr<GetintStmt>;
using PrintfStmtPtr = std::shared_ptr<PrintfStmt>;
using IfStmtPtr = std::shared_ptr<IfStmt>;
using ForStmtPtr = std::shared_ptr<ForStmt>;
using _ForStmtPtr = std::shared_ptr<_ForStmt>;

class FuncDef;
class FuncFParams;
class FuncFParam;
class MainFuncDef;

using FuncDefPtr = std::shared_ptr<FuncDef>;
using FuncFParamsPtr = std::shared_ptr<FuncFParams>;
using FuncFParamPtr = std::shared_ptr<FuncFParam>;
using MainFuncDefPtr = std::shared_ptr<MainFuncDef>;

class CompUnit;
