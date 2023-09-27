//
// Created by hiccup on 2023/9/24.
//

#include "DeclarationParser.hpp"
#include "../expression/ExpressionParser.hpp"

//    <BType>         := 'int'
//    <Decl>          := ['const'] <BType> <Def> { ',' <Def> } ';'    // 'const' 修饰若有，则表示常量
//    <Def>           := Ident { '[' <ConstExp> ']'  } [ '=' <InitVal> ]   // 如果是常量声明则必须有 InitVal
//    <InitVal>       := <ExpInitVal> | <ArrInitVal>
//    <ExpInitVal>    := <Exp>
//    <ArrInitVal>    := '{' [ <InitVal> { ',' <InitVal> } ] '}'    // 语义分析时要求必须个数与维度对应

DeclarationParser::DeclarationParser(TokenStream &tokenStream)
        : tokenStream(tokenStream) {
}

//    <Decl>          := ['const'] <BType> <Def> { ',' <Def> } ';'    // 'const' 修饰若有，则表示常量
DeclPtr DeclarationParser::parseDecl() {
    auto tokenType = tokenStream.peek()->type;
    bool isConst = tokenType == CONSTTK;
    if (isConst) tokenStream.next();
    TokenNode bType(*tokenStream.next());

    std::vector<DefPtr> defPtrs;
    defPtrs.push_back(parseDef(isConst));
    while (!tokenStream.reachEnd()) {
        if (tokenStream.peek()->type != COMMA) {
            break;
        }
        auto comma = tokenStream.next();
        defPtrs.push_back(parseDef(isConst));
    }

    // ;
    auto semicn = tokenStream.next();
    return std::make_shared<Decl>(isConst, bType, defPtrs);
}

//    <Def>           := Ident { '[' <ConstExp> ']'  } [ '=' <InitVal> ]   // 如果是常量声明则必须有 InitVal
DefPtr DeclarationParser::parseDef(bool isConst) {
    // ident
    TokenNode ident(*tokenStream.next());

    std::vector<ConstExpPtr> constExpPtrs;
    ExpressionParser expressionParser(tokenStream);
    while (!tokenStream.reachEnd()) {
        if (tokenStream.peek()->type != LBRACK) break;
        auto left = tokenStream.next();
        constExpPtrs.push_back(expressionParser.parseConstExp());
        auto right = tokenStream.next();
    }
    if (tokenStream.peek()->type == ASSIGN) {
        auto assign = tokenStream.next();
        return std::make_shared<Def>(ident, constExpPtrs, parseInitVal(isConst));
    }
    return std::make_shared<Def>(ident, constExpPtrs, nullptr);
}

//    <InitVal>       := <ExpInitVal> | <ArrInitVal>
InitValPtr DeclarationParser::parseInitVal(bool isConst) {
    if (tokenStream.peek()->type != LBRACE) {
        auto ret = parseExpInitVal(isConst);
        return ret ;
    }
    return parseArrayInitVal(isConst);
}

//    <ExpInitVal>    := <Exp>
ExpInitValPtr DeclarationParser::parseExpInitVal(bool isConst) {
    ExpressionParser expressionParser(tokenStream);
    return std::make_shared<ExpInitVal>(isConst, expressionParser.parseExp());
}

//    <ArrInitVal>    := '{' [ <InitVal> { ',' <InitVal> } ] '}'    // 语义分析时要求必须个数与维度对应
ArrayInitValPtr DeclarationParser::parseArrayInitVal(bool isConst) {
    // '{'
    auto left = tokenStream.next();
    std::vector<InitValPtr> initValPtrs;
    if (tokenStream.peek()->type == RBRACE) {
        // '}'
        auto right = tokenStream.next();
        return std::make_shared<ArrayInitVal>(isConst, initValPtrs);
    }
    initValPtrs.push_back(parseInitVal(isConst));
    while (!tokenStream.reachEnd()) {
        if (tokenStream.peek()->type != COMMA) break;
        tokenStream.next();
        initValPtrs.push_back(parseInitVal(isConst));
    }

    // '}'
    auto right = tokenStream.next();
    return std::make_shared<ArrayInitVal>(isConst, initValPtrs);;
}
















