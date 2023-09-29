//
// Created by hiccup on 2023/9/20.
//

#pragma once

#include "TokenStream.hpp"
#include "expression/Uniform.hpp"
#include "expression/ExpressionParser.hpp"
#include "CompUnit.hpp"
#include "function/FunctionParser.hpp"
#include "declaration/DeclarationParser.hpp"

class Parser {
private:
    TokenStream tokenStream;
public:
    explicit Parser(TokenStream& tokenStream) : tokenStream(tokenStream) {}


    //  <CompUnit>      := { <Decl> } { <FuncDef> } <MainFuncDef>
    CompUnit parseCompUnit() {
        DeclarationParser declarationParser(tokenStream);
        FunctionParser functionParser(tokenStream);
        MainFuncDefPtr _main;
        std::vector<DeclPtr> declPtrs;
        std::vector<FuncDefPtr> funcDefPtrs;
        while (!tokenStream.reachEnd()) {
            auto type = tokenStream.peek()->type;
            auto type1 = tokenStream.peek(1)->type;
            auto type2 = tokenStream.peek(2)->type;
            if (type == INTTK && type1 == MAINTK && type2 == LPARENT) {
                _main = functionParser.parseMainFuncDef();
            } else if ((type == VOIDTK || type == INTTK) && type1 == IDENFR && type2 == LPARENT) {
                funcDefPtrs.push_back(functionParser.parseFuncDef());
            } else {
                declPtrs.push_back(declarationParser.parseDecl());
            }
        }
        return CompUnit(declPtrs, funcDefPtrs, _main);
    }
};