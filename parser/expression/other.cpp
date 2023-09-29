//
// Created by hiccup on 2023/9/22.
//

#include "other.hpp"

#include <utility>


LVal::LVal(TokenNode ident, std::vector<ExpPtr> array) : ident(std::move(ident)), array(std::move(array)) {
    name = "<LVal>";
    printInformation();
}

Exp::Exp(AddExpPtr sharedPtr) : addExpPtr(std::move(sharedPtr)) {
    name = "<Exp>";
    printInformation();
}


Number::Number(std::string num) : num(std::stoi(num)) {
    name = "<Number>";
    printInformation();
}

FuncRParams::FuncRParams(std::vector<ExpPtr> expPtrs) : expPtrs(std::move(expPtrs)) {
    name = "<FuncRParams>";
    printInformation();
}

UnaryExp::UnaryExp(std::vector<TokenType> unaryOps, PrimaryExpPtr primaryExpPtr, FunctionCallPtr functionCallPtr)
        : unaryOps(std::move(unaryOps)), primaryExpPtr(std::move(primaryExpPtr)),
          functionCallPtr(std::move(functionCallPtr)) {
    name = "<UnaryExp>";
    printInformation();
}

FunctionCall::FunctionCall(std::string ident, FuncRParamsPtr funcRParamsPtr)
        : ident(std::move(ident)), funcRParamsPtr(std::move(funcRParamsPtr)) {
    name = "<FunctionCall>";
//    printInformation();
}

ConstExp::ConstExp(AddExpPtr sharedPtr) : addExpPtr(std::move(sharedPtr)){
    name = "<ConstExp>";
    printInformation();
}

Cond::Cond(LOrExpPtr lOrExpPtr) : lOrExpPtr(std::move(lOrExpPtr)) {
    name = "<Cond>";
    printInformation();
}
