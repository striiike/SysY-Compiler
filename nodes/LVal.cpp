//
// Created by hiccup on 2023/10/9.
//

#include "LVal.h"
#include "Exp.h"
#include "../parser/Symbol.hpp"

LVal::LVal(TokenNode ident, std::vector<ExpPtr> array) : ident(std::move(ident)), array(std::move(array)) {
    name = "<LVal>";
    print();
}

void LVal::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
    auto varPtr = symbol.getVar(ident.getValue());
    if (!varPtr)
        errorList.emplace_back(Exception::UNDEFINED_IDENT, ident.getLineNum());
    else if (varPtr->isConst)
        errorList.emplace_back(Exception::CONST_ASSIGNED, ident.getLineNum());
    for (const auto &i: array) {
        i->checkError(ctx, ret);
    }
}