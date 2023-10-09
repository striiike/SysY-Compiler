#include "Exp.h"
#include "UnaryExp.h"
#include "../parser/Symbol.hpp"
#include "TokenNode.h"

Number::Number(std::string num) :
        num(std::stoi(num)) {
    name = "<Number>";
    print();
}

void Number::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
    ASTNode::checkError(ctx, ret);
}

Exp::Exp(AddExpPtr sharedPtr, bool isConst) :
        addExpPtr(std::move(sharedPtr)), isConst(isConst) {
    name = isConst ? "<ConstExp>" : "<Exp>";
    print();
}

void Exp::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
    addExpPtr->checkError(ctx, ret);
}

Cond::Cond(LOrExpPtr lOrExpPtr) : lOrExpPtr(std::move(lOrExpPtr)) {
    name = "<Cond>";
    print();
}

void Cond::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
    lOrExpPtr->checkError(ctx, ret);
}

