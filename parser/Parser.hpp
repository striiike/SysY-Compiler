//
// Created by hiccup on 2023/9/20.
//

#pragma once

#include "TokenStream.hpp"
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

    ConstExpPtr parseConstExp();

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

    //  <CompUnit>      := { <Decl> } { <FuncDef> } <MainFuncDef>
    CompUnit parseCompUnit() {
        MainFuncDefPtr _main;
        std::vector<DeclPtr> declPtrs;
        std::vector<FuncDefPtr> funcDefPtrs;
        while (!tokenStream.reachEnd()) {
            auto type = tokenStream.peek()->type;
            auto type1 = tokenStream.peek(1)->type;
            auto type2 = tokenStream.peek(2)->type;
            if (type == INTTK && type1 == MAINTK && type2 == LPARENT) {
                _main = parseMainFuncDef();
            } else if ((type == VOIDTK || type == INTTK) && type1 == IDENFR && type2 == LPARENT) {
                funcDefPtrs.push_back(parseFuncDef());
            } else {
                declPtrs.push_back(parseDecl());
            }
        }
        return CompUnit(declPtrs, funcDefPtrs, _main);
    }
};