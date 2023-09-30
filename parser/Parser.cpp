//
// Created by hiccup on 2023/9/30.
//
#include "Parser.hpp"
#include <bits/stdc++.h>

using namespace std;

#define CHECK(x, y)



////////////////////////////////////////////////////////////////////////////////////////////////

//    <Decl>          := ['const'] <BType> <Def> { ',' <Def> } ';'     'const' 修饰若有，则表示常量
DeclPtr Parser::parseDecl() {
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
DefPtr Parser::parseDef(bool isConst) {
    // ident
    TokenNode ident(*tokenStream.next());

    std::vector<ConstExpPtr> constExpPtrs;
    while (!tokenStream.reachEnd()) {
        if (tokenStream.peek()->type != LBRACK) break;
        auto left = tokenStream.next();
        constExpPtrs.push_back(parseConstExp());
        auto right = tokenStream.next();
    }
    if (tokenStream.peek()->type == ASSIGN) {
        auto assign = tokenStream.next();
        return std::make_shared<Def>(isConst, ident, constExpPtrs, parseInitVal(isConst));
    }
    return std::make_shared<Def>(isConst, ident, constExpPtrs, nullptr);
}

//    <InitVal>       := <ExpInitVal> | <ArrInitVal>
InitValPtr Parser::parseInitVal(bool isConst) {
    if (tokenStream.peek()->type != LBRACE) {
        auto ret = parseExpInitVal(isConst);
        return ret;
    }
    return parseArrayInitVal(isConst);
}

//    <ExpInitVal>    := <Exp>
ExpInitValPtr Parser::parseExpInitVal(bool isConst) {
    if (isConst) return std::make_shared<ExpInitVal>(isConst, nullptr, parseConstExp());
    return std::make_shared<ExpInitVal>(isConst, parseExp(), nullptr);
}

//    <ArrInitVal>    := '{' [ <InitVal> { ',' <InitVal> } ] '}'    // 语义分析时要求必须个数与维度对应
ArrayInitValPtr Parser::parseArrayInitVal(bool isConst) {
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
    return std::make_shared<ArrayInitVal>(isConst, initValPtrs);
}


//
//    <LVal>          := Ident { '[' <Exp> ']' } // public class LVal { ident, class Index, List<Index> }
//    <PrimaryExp>    := '(' <Exp> ')' | <LVal> | <Number>
//          Look forward: '(' :: <SubExp>, <Ident> :: <LVal>, <IntConst> :: <Number>
//    <Number>        := IntConst   // 该节点只存一个 Token
//    <FunctionCall>  := <Ident> '(' [ <FuncRParams> ] ')'
//    <FuncRParams>   := <Exp> { ',', <Exp> } // List<Exp>
//    <UnaryExp>      := { <UnaryOp> } (<PrimaryExp> | <FunctionCall>)
//          List<UnaryOp>, UnaryOp 包含在 UnaryExp 内部，不单独建类
//          即不包含 <UnaryOp> 的 <UnaryExp>    需要向前看 2 个符号:
//          Ident '(' :: <FunctionCall>, Ident :: <LVal>, '(' :: <SubExp>, IntConst :: <Number>

ExpPtr Parser::parseExp() {
    return std::make_shared<Exp>(std::move(parseAddExp()));
}

ConstExpPtr Parser::parseConstExp() {
    return std::make_shared<ConstExp>(std::move(parseAddExp()));
}

CondPtr Parser::parseCond() {
    return std::make_shared<Cond>(std::move(parseLOrExp()));
}

LValPtr Parser::parseLVal() {
    vector<ExpPtr> array;
    TokenNode ident(*tokenStream.next());
    while (!tokenStream.reachEnd()) {
        auto token = tokenStream.peek();
        if (!token || (token->type != LBRACK)) break;

        Token leftBracket = *tokenStream.next();
        ExpPtr expPtr = parseExp();

        // pass right
        Token right = *tokenStream.next();
        array.push_back(expPtr);
    }
    return std::make_shared<LVal>(ident, array);
}

PrimaryExpPtr Parser::parsePrimaryExp() {
    auto token = tokenStream.peek();

    switch (token->type) {
        case LPARENT: {
            auto left = tokenStream.next();
            ExpPtr expPtr = parseExp();
            auto right = tokenStream.next();
            return std::make_shared<PrimaryExp>(expPtr, nullptr, nullptr);
        }
        case IDENFR:
            return std::make_shared<PrimaryExp>(nullptr, parseLVal(), nullptr);
        case INTCON:
            return std::make_shared<PrimaryExp>(nullptr, nullptr, parseNumber());
        default:
            return nullptr;
    }
}

NumberPtr Parser::parseNumber() {
    string str = tokenStream.next()->value;
    return make_shared<Number>(str);
}

FunctionCallPtr Parser::parseFunctionCall() {
    string ident = tokenStream.next()->value;
    auto left = tokenStream.next();

    /// a little bit \silly
    auto token = tokenStream.peek()->type;
    if (token == RPARENT) {
        auto right = tokenStream.next();
        return make_shared<FunctionCall>(ident, nullptr);
    } else if (token == IDENFR || token == LPARENT || token == INTCON
               || token == PLUS || token == MINU || token == NOT) {
        FuncRParamsPtr funcRParamsPtr = parseFuncRParams();
        tokenStream.next();
        return make_shared<FunctionCall>(ident, funcRParamsPtr);
    } else {
        /// \error
        return nullptr;
    }


}

FuncRParamsPtr Parser::parseFuncRParams() {
    vector<ExpPtr> expPtrs;
    expPtrs.push_back(parseExp());
    while (!tokenStream.reachEnd()) {
        if (tokenStream.peek()->type != COMMA) {
            break;
        }
        tokenStream.next();
        expPtrs.push_back(parseExp());
    }
    return make_shared<FuncRParams>(expPtrs);
}

UnaryExpPtr Parser::parseUnaryExp() {
    std::vector<TokenType> unaryOps;
    while (!tokenStream.reachEnd()) {
        if (tokenStream.peek()->type != NOT && tokenStream.peek()->type != PLUS && tokenStream.peek()->type != MINU) {
            break;
        }
        unaryOps.push_back(tokenStream.next()->type);
        printString("<UnaryOp>");
    }

    auto token1 = tokenStream.peek()->type;
    auto token2 = tokenStream.peek(1)->type;
    if (token1 == IDENFR && token2 == LPARENT) {
        auto ret = make_shared<UnaryExp>(unaryOps, nullptr, parseFunctionCall());
        for (int i = 0; i < unaryOps.size(); i++) {
            printString("<UnaryExp>");
        }
        return ret;
    } else {
        auto ret = make_shared<UnaryExp>(unaryOps, parsePrimaryExp(), nullptr);
        for (int i = 0; i < unaryOps.size(); i++) {
            printString("<UnaryExp>");
        }
        return ret;
    }

}

MulExpPtr Parser::parseMulExp() {
    auto leftOperand = parseUnaryExp();
    vector<TokenType> operators;
    vector<UnaryExpPtr> operands;

    while (!tokenStream.reachEnd()) {
        auto token = tokenStream.peek();
        if (!token || (token->type != MULT && token->type != DIV && token->type != MOD)) break;
        printString("<MulExp>");
        operators.push_back(tokenStream.next()->type);
        operands.push_back(parseUnaryExp());


    }

    return std::make_shared<MulExp>(std::move(leftOperand), std::move(operators), std::move(operands));
}

AddExpPtr Parser::parseAddExp() {
    auto leftOperand = parseMulExp();
    vector<TokenType> operators;
    vector<MulExpPtr> operands;

    while (!tokenStream.reachEnd()) {
        auto token = tokenStream.peek();
        if (!token || (token->type != PLUS && token->type != MINU)) break;
        printString("<AddExp>");

        operators.push_back(tokenStream.next()->type); // Assuming next() will return the token
        operands.push_back(parseMulExp());

    }

    return std::make_shared<AddExp>(std::move(leftOperand), std::move(operators), std::move(operands));
}

RelExpPtr Parser::parseRelExp() {
    auto leftOperand = parseAddExp();
    vector<TokenType> operators;
    vector<AddExpPtr> operands;

    while (!tokenStream.reachEnd()) {
        auto token = tokenStream.peek();
        if (!token || (token->type != LSS && token->type != GRE && token->type != LEQ && token->type != GEQ)) break;
        printString("<RelExp>");

        operators.push_back(tokenStream.next()->type);
        operands.push_back(parseAddExp());

    }

    return std::make_shared<RelExp>(std::move(leftOperand), std::move(operators), std::move(operands));
}

EqExpPtr Parser::parseEqExp() {
    auto leftOperand = parseRelExp();
    vector<TokenType> operators;
    vector<RelExpPtr> operands;

    while (!tokenStream.reachEnd()) {
        auto token = tokenStream.peek();
        if (!token || (token->type != EQL && token->type != NEQ)) break;
        printString("<EqExp>");

        operators.push_back(tokenStream.next()->type);
        operands.push_back(parseRelExp());
    }

    return std::make_shared<EqExp>(std::move(leftOperand), std::move(operators), std::move(operands));
}

LAndExpPtr Parser::parseLAndExp() {
    auto leftOperand = parseEqExp();
    vector<TokenType> operators;
    vector<EqExpPtr> operands;

    while (!tokenStream.reachEnd()) {
        auto token = tokenStream.peek();
        if (!token || token->type != AND) break;
        printString("<LAndExp>");

        operators.push_back(tokenStream.next()->type);
        operands.push_back(parseEqExp());
    }

    return std::make_shared<LAndExp>(std::move(leftOperand), std::move(operators), std::move(operands));
}

LOrExpPtr Parser::parseLOrExp() {
    auto leftOperand = parseLAndExp();
    vector<TokenType> operators;
    vector<LAndExpPtr> operands;

    while (!tokenStream.reachEnd()) {
        auto token = tokenStream.peek();
        if (!token || token->type != OR) break;
        printString("<LOrExp>");

        operators.push_back(tokenStream.next()->type);
        operands.push_back(parseLAndExp());
    }

    return std::make_shared<LOrExp>(std::move(leftOperand), std::move(operators), std::move(operands));
}


//    <FuncDef>       := <FuncType> Ident '(' [<FuncFParams> ] ')' <Block>
//    <MainFuncDef>   := 'int' 'main' '(' ')' <Block>
//    <FuncType>      := 'void' | 'int'
//    <FuncFParams>   := <FuncFParam> { ',' <FuncFParam> }
//    <FuncFParam>    := <BType> Ident [ '[' ']' { '[' <ConstExp> ']' } ]

FuncDefPtr Parser::parseFuncDef() {
    TokenNode funcType(tokenStream.next().value());
    printString("<FuncType>");
    TokenNode ident(tokenStream.next().value());
    auto left = tokenStream.next();
    FuncFParamsPtr funcFParamsPtr = nullptr;
    if (tokenStream.peek()->type != RPARENT) {
        funcFParamsPtr = parseFuncFParams();
    }
    auto right = tokenStream.next();
    return std::make_shared<FuncDef>(funcType, ident, funcFParamsPtr, parseBlock());
}

MainFuncDefPtr Parser::parseMainFuncDef() {
    TokenNode _int(tokenStream.next().value());
    TokenNode _main(tokenStream.next().value());
    auto left = tokenStream.next();
    auto right = tokenStream.next();

    return std::make_shared<MainFuncDef>(_int, _main, parseBlock());
}

FuncFParamsPtr Parser::parseFuncFParams() {
    std::vector<FuncFParamPtr> funcFParamPtrs;
    funcFParamPtrs.push_back(parseFuncFParam());
    while (!tokenStream.reachEnd()) {
        if (tokenStream.peek()->type != COMMA) break;
        auto comma = tokenStream.next();
        funcFParamPtrs.push_back(parseFuncFParam());
    }
    return std::make_shared<FuncFParams>(funcFParamPtrs);
}

FuncFParamPtr Parser::parseFuncFParam() {
    Token _bType = tokenStream.next().value();
    TokenNode bType(_bType);
    Token _ident = tokenStream.next().value();
    TokenNode ident(_ident);

    std::vector<ConstExpPtr> constExpPtrs;

    if (tokenStream.peek()->type != LBRACK)
        return std::make_shared<FuncFParam>(bType, ident, false, constExpPtrs);
    auto left = tokenStream.next();
    auto right = tokenStream.next();
    while (!tokenStream.reachEnd()) {
        if (tokenStream.peek()->type != LBRACK) break;
        auto _left = tokenStream.next();
        constExpPtrs.push_back(parseConstExp());
        auto _right = tokenStream.next();
    }
    return std::make_shared<FuncFParam>(bType, ident, true, constExpPtrs);
}

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


// <Block>         := '{' { <BlockItem> } '}'
BlockPtr Parser::parseBlock() {
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
BlockItemPtr Parser::parseBlockItem() {
    TokenType token = tokenStream.peek()->type;
    if (token == CONSTTK || token == INTTK) {
        return std::make_shared<BlockItem>(parseDecl(), nullptr);
    }
    return std::make_shared<BlockItem>(nullptr, parseStmt());
}

//    <Stmt>          := ';' | <SimpleStmt> ';' | <ComplexStmt>    // 将分号放在这里统一处理
StmtPtr Parser::parseStmt() {
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

/// used to check whether Exp is LVal, \only for parseSimpleStmt
LValPtr getLValPtr(const ExpPtr &expPtr) {
    auto add = expPtr->addExpPtr;
    auto mul = add->getLeftOperand();
    auto unary = mul->getLeftOperand();
    auto ops = unary->getUnaryOps();
    if (!ops.empty()) return nullptr;
    auto primary = unary->getPrimaryExpPtr();
    if (primary)
        return primary->getLValPtr();
    else return nullptr;
}


SimpleStmtPtr Parser::parseAssignOrGetintStmt(const LValPtr &lValPtr) {
    auto assign = tokenStream.next();
    if (tokenStream.peek()->type == GETINTTK) {
        auto _getint = *tokenStream.next();
        tokenStream.next();
        tokenStream.next();
        return std::make_shared<GetintStmt>(lValPtr, _getint);
    }
    return std::make_shared<AssignStmt>(lValPtr, parseExp());
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
SimpleStmtPtr Parser::parseSimpleStmt() {
    auto token = *tokenStream.peek();
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
            expPtr = parseExp();
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
            expPtrs.push_back(parseExp());
        }
        auto right = tokenStream.next();
        return std::make_shared<PrintfStmt>(_printf, formatStringToken, expPtrs);
    }


    /// \very \servere \problem \!!
    bool isAssignOrGetint = false;
    PARSER_DISPLAY = false;
    {
        /// \NO_NEED_TO_WORRY_SIDE_EFFECT_OF_COPY
        TokenStream tokenStreamCopy1(tokenStream);
        TokenStream tokenStreamCopy2(tokenStream);

        Parser ParserCopy1(tokenStreamCopy1);
        Parser ParserCopy2(tokenStreamCopy2);
        ExpPtr expPtr = ParserCopy1.parseExp();
        LValPtr lValPtr = getLValPtr(expPtr);
        if (lValPtr) {
            LValPtr lValPtr1 = ParserCopy2.parseLVal();
            auto fuck = tokenStreamCopy2.peek();
            isAssignOrGetint = (tokenStreamCopy2.peek()->type == ASSIGN);
        }
    }
    PARSER_DISPLAY = true;

    if (isAssignOrGetint) {
        return parseAssignOrGetintStmt(parseLVal());
    } else {
        return std::make_shared<ExpStmt>(parseExp());
    }
}

_ForStmtPtr Parser::parse_ForStmt() {
    LValPtr lValPtr = parseLVal();
    auto assign = tokenStream.next();
    return std::make_shared<_ForStmt>(lValPtr, parseExp());
}

//    <IfStmt>        := 'if' '(' <Cond> ')' <Stmt> [ 'else' <Stmt> ]
//    <WhileStmt>     := 'while' '(' <Cond> ')' <Stmt>
//    <ComplexStmt>       := <IfStmt> | <WhileStmt> | <Block>
ComplexStmtPtr Parser::parseComplexStmt() {
    if (tokenStream.peek()->type == IFTK) {
        auto _if = tokenStream.next();
        auto left = tokenStream.next();
        CondPtr condPtr = parseCond();
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
            condPtr = parseCond();
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
