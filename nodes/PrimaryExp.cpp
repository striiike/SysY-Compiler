//
// Created by hiccup on 2023/10/7.
//

#include "PrimaryExp.h"
#include "Exp.h"
#include "LVal.h"

PrimaryExp::PrimaryExp(ExpPtr exp, LValPtr lVal, NumberPtr number)
        : expPtr(std::move(exp)), lValPtr(std::move(lVal)),
          numberPtr(std::move(number)) {
    name = "<PrimaryExp>";
    print();
}

LValPtr PrimaryExp::getLValPtr() { return lValPtr; }

NumberPtr PrimaryExp::getNumberPtr() { return numberPtr; }

ExpPtr PrimaryExp::getExpPtr() { return expPtr; }

void PrimaryExp::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
    auto _ret = make_shared<ErrorRet>();

    if (expPtr) expPtr->checkError(ctx, _ret);
    else if (lValPtr) lValPtr->checkError(ctx, _ret);
    else if (numberPtr) numberPtr->checkError(ctx, _ret);

    ret->dim = _ret->dim;
}
