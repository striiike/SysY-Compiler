//
// Created by hiccup on 2023/9/21.
//

#include "ExpressionParser.hpp"
#include "../TokenNode.hpp"
#include "other.hpp"

using namespace std;

ExpressionParser::ExpressionParser(TokenStream &tokenStream) : tokenStream(tokenStream) {}

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

ExpPtr ExpressionParser::parseExp() {
    return std::make_shared<Exp>(std::move(parseAddExp()));
}

ConstExpPtr ExpressionParser::parseConstExp() {
    return std::make_shared<ConstExp>(std::move(parseAddExp()));
}

CondPtr ExpressionParser::parseCond() {
    return std::make_shared<Cond>(std::move(parseLOrExp()));
}

LValPtr ExpressionParser::parseLVal() {
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

PrimaryExpPtr ExpressionParser::parsePrimaryExp() {
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

NumberPtr ExpressionParser::parseNumber() {
    string str = tokenStream.next()->value;
    return make_shared<Number>(str);
}

FunctionCallPtr ExpressionParser::parseFunctionCall() {
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

FuncRParamsPtr ExpressionParser::parseFuncRParams() {
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

UnaryExpPtr ExpressionParser::parseUnaryExp() {
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

MulExpPtr ExpressionParser::parseMulExp() {
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

AddExpPtr ExpressionParser::parseAddExp() {
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

RelExpPtr ExpressionParser::parseRelExp() {
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

EqExpPtr ExpressionParser::parseEqExp() {
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

LAndExpPtr ExpressionParser::parseLAndExp() {
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

LOrExpPtr ExpressionParser::parseLOrExp() {
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


