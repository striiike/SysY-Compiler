//
// Created by hiccup on 2023/10/7.
//

#include "UnaryExp.h"
#include "PrimaryExp.h"
#include "Exp.h"
#include "FunctionCall.h"

UnaryExp::UnaryExp(std::vector<TokenType> unaryOps, PrimaryExpPtr primaryExpPtr, FunctionCallPtr functionCallPtr)
        : unaryOps(std::move(unaryOps)), primaryExpPtr(std::move(primaryExpPtr)),
          functionCallPtr(std::move(functionCallPtr)) {
    name = "<UnaryExp>";
    print();
}

const std::vector<TokenType> &UnaryExp::getUnaryOps() const {
    return unaryOps;
}

const PrimaryExpPtr &UnaryExp::getPrimaryExpPtr() const {
    return primaryExpPtr;
}

const FunctionCallPtr &UnaryExp::getFunctionCallPtr() const {
    return functionCallPtr;
}

void UnaryExp::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
    auto _ret = make_shared<ErrorRet>();
    if (primaryExpPtr) {
        primaryExpPtr->checkError(ctx, _ret);
    }
    else if (functionCallPtr) {
        functionCallPtr->checkError(ctx, _ret);
    }
    ret->dim = _ret->dim;
}
