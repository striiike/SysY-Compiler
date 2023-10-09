//
// Created by hiccup on 2023/10/9.
//

#include "FunctionCall.h"
#include "Exp.h"
#include "../parser/Symbol.hpp"

FuncRParams::FuncRParams(std::vector<ExpPtr> expPtrs) : expPtrs(std::move(expPtrs)) {
    name = "<FuncRParams>";
    print();
}

void FuncRParams::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
    for (const auto &i: expPtrs) {
        i->checkError(ctx, ret);
    }
}

FunctionCall::FunctionCall(TokenNode ident, FuncRParamsPtr funcRParamsPtr)
        : ident(std::move(ident)), funcRParamsPtr(std::move(funcRParamsPtr)) {
    name = "<FunctionCall>";
}

void FunctionCall::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
    auto funcPtr = symbol.getFunc(ident.getValue());
    if (!funcPtr) errorList.emplace_back(Exception::UNDEFINED_IDENT, ident.getLineNum());
    if (funcRParamsPtr)
        funcRParamsPtr->checkError(ctx, ret);
}

