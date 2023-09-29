//
// Created by hiccup on 2023/9/21.
//

#pragma once

#include <memory>
#include "../TokenStream.hpp"
#include "Uniform.hpp"


class ExpressionParser {
private:
    TokenStream &tokenStream;
public:
    explicit ExpressionParser(TokenStream &tokenStream);

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
};

