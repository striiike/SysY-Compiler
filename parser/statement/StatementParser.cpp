//
// Created by hiccup on 2023/9/26.
//

#include "StatementParser.hpp"
#include "../declaration/DeclarationParser.hpp"
#include "../expression/ExpressionParser.hpp"
#include "../expression/other/other.hpp"
#include "../expression/uniform/Uniform.hpp"


//    // 以分号结尾的简单语句
//    <AssignStmt>    := <LVal> '=' <Exp> // 这些新增非终结符的组成不含分号
//    <ExpStmt>       := <Exp>            // 空语句放到最后了
//    <BreakStmt>     := 'break'
//    <ContinueStmt>  := 'continue'
//    <ReturnStmt>    := 'return' [<Exp>]
//    <GetintStmt>     := <LVal> '=' 'getint' '(' ')'
//    <PrintfStmt>    := 'printf' '(' FormatString { ',' <Exp> } ')'
//    <SimpleStmt>       := <AssignStmt> | <ExpStmt> | <BreakStmt> | <ContinueStmt>
//    | <ReturnStmt> | <GetintStmt> | <PrintfStmt> // <SimpleStmt> 是以分号结尾的语句(不含分号)的合集
//    // 复杂的语句
//    <IfStmt>        := 'if' '(' <Cond> ')' <Stmt> [ 'else' <Stmt> ]
//    <WhileStmt>     := 'while' '(' <Cond> ')' <Stmt>
//    <ComplexStmt>       := <IfStmt> | <WhileStmt> | <Block>
//
//    <Stmt>          := ';' | <SimpleStmt> ';' | <ComplexStmt>    // 将分号放在这里统一处理
//
//    <BlockItem>     := <Decl> | <Stmt>
//
//    <Block>         := '{' { <BlockItem> } '}'


StatementParser::StatementParser(TokenStream &tokenStream) :
        tokenStream(tokenStream) {}

// <Block>         := '{' { <BlockItem> } '}'
BlockPtr StatementParser::parseBlock() {
    auto left = tokenStream.next();
    std::vector<BlockItemPtr> blockItemPtrs;
    while (!tokenStream.reachEnd()) {
        if (tokenStream.peek()->type == RBRACE) break;
        blockItemPtrs.push_back(parseBlockItem());
    }
    auto right = tokenStream.next();
    return std::make_shared<Block>(blockItemPtrs);
}

//    <BlockItem>     := <Decl> | <Stmt>
BlockItemPtr StatementParser::parseBlockItem() {
    TokenType token = tokenStream.peek()->type;
    if (token == CONSTTK || token == INTTK) {
        DeclarationParser declarationParser(tokenStream);
        return std::make_shared<BlockItem>(declarationParser.parseDecl(), nullptr);
    }
    return std::make_shared<BlockItem>(nullptr, parseStmt());
}

//    <Stmt>          := ';' | <SimpleStmt> ';' | <ComplexStmt>    // 将分号放在这里统一处理
StmtPtr StatementParser::parseStmt() {
    if (tokenStream.peek()->type == SEMICN) {
        auto semicn = tokenStream.next();
        return std::make_shared<Stmt>(nullptr, nullptr);
    }
    if (tokenStream.peek()->type == IFTK || tokenStream.peek()->type == FORTK || tokenStream.peek()->type == LBRACE)
        return std::make_shared<Stmt>(nullptr, parseComplexStmt());

    auto simpleStmtPtr = parseSimpleStmt();
    auto semicn = tokenStream.next();
    return std::make_shared<Stmt>(simpleStmtPtr, nullptr);
}

// used to check whether Exp is LVal, \only for parseSimpleStmt
LValPtr getLValPtr(const ExpPtr &expPtr) {
    ;
    auto add = expPtr->addExpPtr;
    auto mul = add->getLeftOperand();
    auto unary = mul->getLeftOperand();
    auto ops = unary->getUnaryOps();
    if (!ops.empty()) return nullptr;
    auto primary = unary->getPrimaryExpPtr();
    if (primary)
        return primary->getLValPtr();
    else return nullptr;
};


SimpleStmtPtr StatementParser::parseAssignOrGetintStmt(const LValPtr &lValPtr) {
    auto assign = tokenStream.next();
    if (tokenStream.peek()->type == GETINTTK) {
        auto _getint = *tokenStream.next();
        tokenStream.next();
        tokenStream.next();
        return std::make_shared<GetintStmt>(lValPtr, _getint);
    }
    ExpressionParser expressionParser(tokenStream);
    return std::make_shared<AssignStmt>(lValPtr, expressionParser.parseExp());
}

//    <AssignStmt>    := <LVal> '=' <Exp> // 这些新增非终结符的组成不含分号
//    <ExpStmt>       := <Exp>            // 空语句放到最后了
//    <BreakStmt>     := 'break'
//    <ContinueStmt>  := 'continue'
//    <ReturnStmt>    := 'return' [<Exp>]
//    <GetintStmt>     := <LVal> '=' 'getint' '(' ')'
//    <PrintfStmt>    := 'printf' '(' FormatString { ',' <Exp> } ')'
//    <SimpleStmt>       := <AssignStmt> | <ExpStmt> | <BreakStmt> | <ContinueStmt>
//    | <ReturnStmt> | <GetintStmt> | <PrintfStmt>
SimpleStmtPtr StatementParser::parseSimpleStmt() {
    auto token = *tokenStream.peek();
    ExpressionParser expressionParser(tokenStream);
    if (token.type == BREAKTK) {
        TokenNode _break(token);
        tokenStream.next();
        return std::make_shared<BreakStmt>(_break);
    }
    if (token.type == CONTINUETK) {
        TokenNode _continue(token);
        tokenStream.next();
        return std::make_shared<ContinueStmt>(_continue);
    }
    if (token.type == RETURNTK) {
        TokenNode _return(token);
        tokenStream.next();
        ExpPtr expPtr = nullptr;
        if (tokenStream.peek()->type != SEMICN) {
            expPtr = expressionParser.parseExp();
        }
        return std::make_shared<ReturnStmt>(_return, expPtr);
    }
    if (token.type == PRINTFTK) {
        TokenNode _printf(token);
        tokenStream.next();
        auto left = tokenStream.next();
        Token formatStringToken = *tokenStream.next();
        std::vector<ExpPtr> expPtrs;
        while (!tokenStream.reachEnd()) {
            if (tokenStream.peek()->type != COMMA) break;
            tokenStream.next();
            expPtrs.push_back(expressionParser.parseExp());
        }
        auto right = tokenStream.next();
        return std::make_shared<PrintfStmt>(_printf, formatStringToken, expPtrs);
    }


    /// \very \servere \problem \!!
    bool isAssignOrGetint = false;
    PARSER_DISPLAY = false;
    {
        /// \NO_NEED_TO_WORRY_SIDE_EFFECT_OF_COPY
        /// \ZHEN_NI_MA_CHUN
        TokenStream tokenStreamCopy1(tokenStream);
        TokenStream tokenStreamCopy2(tokenStream);

        ExpressionParser expressionParserCopy1(tokenStreamCopy1);
        ExpressionParser expressionParserCopy2(tokenStreamCopy2);
        ExpPtr expPtr = expressionParserCopy1.parseExp();
        LValPtr lValPtr = getLValPtr(expPtr);
        if (lValPtr) {
            LValPtr lValPtr1 = expressionParserCopy2.parseLVal();
            isAssignOrGetint = (tokenStreamCopy2.peek()->type == ASSIGN);
        }
    }
    PARSER_DISPLAY = true;

    if (isAssignOrGetint) {
        return parseAssignOrGetintStmt(expressionParser.parseLVal());
    } else {
        return std::make_shared<ExpStmt>(expressionParser.parseExp());
    }
}

_ForStmtPtr StatementParser::parse_ForStmt() {
    ExpressionParser expressionParser(tokenStream);
    LValPtr lValPtr = expressionParser.parseLVal();
    auto assign = tokenStream.next();
    return std::make_shared<_ForStmt>(lValPtr, expressionParser.parseExp());
}

//    <IfStmt>        := 'if' '(' <Cond> ')' <Stmt> [ 'else' <Stmt> ]
//    <WhileStmt>     := 'while' '(' <Cond> ')' <Stmt>
//    <ComplexStmt>       := <IfStmt> | <WhileStmt> | <Block>
ComplexStmtPtr StatementParser::parseComplexStmt() {
    if (tokenStream.peek()->type == IFTK) {
        auto _if = tokenStream.next();
        auto left = tokenStream.next();
        ExpressionParser expressionParser(tokenStream);
        CondPtr condPtr = expressionParser.parseCond();
        auto right = tokenStream.next();
        StmtPtr stmtPtr1 = parseStmt();
        if (tokenStream.peek()->type == ELSETK) {
            auto _else = tokenStream.next();
            return std::make_shared<IfStmt>(condPtr, stmtPtr1, parseStmt());
        }
    }
    if (tokenStream.peek()->type == FORTK) {
        auto _for = tokenStream.next();
        auto left = tokenStream.next();
        _ForStmtPtr assignStmtPtr1 = nullptr;
        if (tokenStream.peek()->type != SEMICN) {
            assignStmtPtr1 = parse_ForStmt();
        }
        auto semicn1 = tokenStream.next();
        CondPtr condPtr = nullptr;
        if (tokenStream.peek()->type != SEMICN) {
            ExpressionParser expressionParser(tokenStream);
            condPtr = expressionParser.parseCond();
        }
        auto semicn2 = tokenStream.next();
        _ForStmtPtr assignStmtPtr2 = nullptr;

        if (tokenStream.peek()->type != RPARENT) {
            assignStmtPtr1 = parse_ForStmt();
        }
        auto right = tokenStream.next();
        return std::make_shared<ForStmt>(condPtr, parseStmt(), assignStmtPtr1, assignStmtPtr2);
    }
    if (tokenStream.peek()->type == LBRACE) {
        return parseBlock();
    }

    /// \error
    return nullptr;
}