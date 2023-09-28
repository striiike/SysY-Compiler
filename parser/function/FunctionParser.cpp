//
// Created by hiccup on 2023/9/26.
//

#include "FunctionParser.hpp"
#include "../expression/ExpressionParser.hpp"
#include "../statement/StatementParser.hpp"

//    <FuncDef>       := <FuncType> Ident '(' [<FuncFParams> ] ')' <Block>
//    <MainFuncDef>   := 'int' 'main' '(' ')' <Block>
//    <FuncType>      := 'void' | 'int'
//    <FuncFParams>   := <FuncFParam> { ',' <FuncFParam> }
//    <FuncFParam>    := <BType> Ident [ '[' ']' { '[' <ConstExp> ']' } ]

FunctionParser::FunctionParser(TokenStream &tokenStream)
        : tokenStream(tokenStream) {}

FuncDefPtr FunctionParser::parseFuncDef() {
    TokenNode funcType(tokenStream.next().value());
    printString("<FuncType>");
    TokenNode ident(tokenStream.next().value());
    auto left = tokenStream.next();
    FuncFParamsPtr funcFParamsPtr = nullptr;
    if (tokenStream.peek()->type != RPARENT) {
        funcFParamsPtr = parseFuncFParams();
    }
    auto right = tokenStream.next();
    StatementParser statementParser(tokenStream);
    return std::make_shared<FuncDef>(funcType, ident, funcFParamsPtr, statementParser.parseBlock());
}

MainFuncDefPtr FunctionParser::parseMainFuncDef() {
    TokenNode _int(tokenStream.next().value());
    TokenNode _main(tokenStream.next().value());
    auto left = tokenStream.next();
    auto right = tokenStream.next();

    StatementParser statementParser(tokenStream);
    return std::make_shared<MainFuncDef>(_int, _main, statementParser.parseBlock());
}

FuncFParamsPtr FunctionParser::parseFuncFParams() {
    std::vector<FuncFParamPtr> funcFParamPtrs;
    funcFParamPtrs.push_back(parseFuncFParam());
    while (!tokenStream.reachEnd()) {
        if (tokenStream.peek()->type != COMMA) break;
        auto comma = tokenStream.next();
        funcFParamPtrs.push_back(parseFuncFParam());
    }
    return std::make_shared<FuncFParams>(funcFParamPtrs);
}

FuncFParamPtr FunctionParser::parseFuncFParam() {
    Token _bType = tokenStream.next().value();
    TokenNode bType(_bType);
    Token _ident = tokenStream.next().value();
    TokenNode ident(_ident);

    std::vector<ConstExpPtr> constExpPtrs;
    ExpressionParser expressionParser(tokenStream);

    if (tokenStream.peek()->type != LBRACK)
        return std::make_shared<FuncFParam>(bType, ident, false, constExpPtrs);
    auto left = tokenStream.next();
    auto right = tokenStream.next();
    while (!tokenStream.reachEnd()) {
        if (tokenStream.peek()->type != LBRACK) break;
        auto _left = tokenStream.next();
        constExpPtrs.push_back(expressionParser.parseConstExp());
        auto _right = tokenStream.next();
    }
    return std::make_shared<FuncFParam>(bType, ident, true, constExpPtrs);
}