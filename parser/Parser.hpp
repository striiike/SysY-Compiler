//
// Created by hiccup on 2023/9/20.
//

#pragma once

#include "TokenStream.hpp"
#include "Exception.hpp"
#include "../nodes/lib.hpp"
#include <bits/stdc++.h>

class Parser {
private:
    TokenStream &tokenStream;
public:
    explicit Parser(TokenStream &tokenStream) : tokenStream(tokenStream) {}

    std::shared_ptr<Decl> parseDecl();

    std::shared_ptr<Def> parseDef(bool isConst);

    InitValPtr parseInitVal(bool isConst);

    ExpInitValPtr parseExpInitVal(bool isConst);

    ArrayInitValPtr parseArrayInitVal(bool isConst);

    AddExpPtr parseAddExp();

    MulExpPtr parseMulExp();

    RelExpPtr parseRelExp();

    EqExpPtr parseEqExp();

    LAndExpPtr parseLAndExp();

    LOrExpPtr parseLOrExp();

    UnaryExpPtr parseUnaryExp();

    LValPtr parseLVal();

    PrimaryExpPtr parsePrimaryExp();

    NumberPtr parseNumber();

    FunctionCallPtr parseFunctionCall();

    FuncRParamsPtr parseFuncRParams();

    ExpPtr parseExp();

    ExpPtr parseConstExp();

    CondPtr parseCond();

    FuncDefPtr parseFuncDef();

    MainFuncDefPtr parseMainFuncDef();

    FuncFParamsPtr parseFuncFParams();

    FuncFParamPtr parseFuncFParam();

    BlockPtr parseBlock();

    BlockItemPtr parseBlockItem();

    StmtPtr parseStmt();

    SimpleStmtPtr parseSimpleStmt();

    ComplexStmtPtr parseComplexStmt();

    _ForStmtPtr parse_ForStmt();

    SimpleStmtPtr parseAssignOrGetintStmt(const LValPtr &lValPtr);

    CompUnit parseCompUnit();
};