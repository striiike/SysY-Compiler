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
    if (expPtr) expPtr->checkError(ctx, ret);
    if (lValPtr) lValPtr->checkError(ctx, ret);
    if (numberPtr) numberPtr->checkError(ctx, ret);
}
