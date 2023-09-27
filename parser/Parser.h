//
// Created by hiccup on 2023/9/20.
//

#pragma once

#include "TokenStream.h"
#include "expression/uniform/Uniform.hpp"
#include "expression/ExpressionParser.hpp"
#include "CompUnit.hpp"

class Parser {
private:
    TokenStream tokenStream;
    ExpressionParser expressionParser;
public:
    explicit Parser(TokenStream& tokenStream);


    CompUnit parseCompUnit();
};