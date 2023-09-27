//
// Created by hiccup on 2023/9/26.
//

#pragma once

#include "../TokenStream.h"
#include "FuncDef.hpp"
#include "FuncFParam.hpp"
#include "FuncFParams.hpp"
#include "MainFuncDef.hpp"

class FunctionParser {
private:
    TokenStream &tokenStream;
public :
    FunctionParser(TokenStream &tokenStream);

    FuncDefPtr parseFuncDef();

    MainFuncDefPtr parseMainFuncDef();

    FuncFParamsPtr parseFuncFParams();

    FuncFParamPtr parseFuncFParam();
};

