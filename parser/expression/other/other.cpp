//
// Created by hiccup on 2023/9/22.
//

#include "other.hpp"

#include <utility>


LVal::LVal(TokenNode ident, std::vector<ExpPtr> array) : ident(std::move(ident)), array(std::move(array)) {}

Exp::Exp(AddExpPtr sharedPtr) : addExpPtr(std::move(sharedPtr)) {}


Number::Number(std::string num) : num(std::stoi(num)) {
}

FuncRParams::FuncRParams(std::vector<ExpPtr> expPtrs) : expPtrs(std::move(expPtrs)) {
}

UnaryExp::UnaryExp(std::vector<TokenType> unaryOps, PrimaryExpPtr primaryExpPtr, FunctionCallPtr functionCallPtr)
        : unaryOps(std::move(unaryOps)), primaryExpPtr(std::move(primaryExpPtr)),
          functionCallPtr(std::move(functionCallPtr)) {}

FunctionCall::FunctionCall(std::string ident, FuncRParamsPtr funcRParamsPtr)
        : ident(std::move(ident)), funcRParamsPtr(std::move(funcRParamsPtr)) {}

ConstExp::ConstExp(AddExpPtr sharedPtr) : addExpPtr(std::move(sharedPtr)){

}

Cond::Cond(LOrExpPtr lOrExpPtr) : lOrExpPtr(std::move(lOrExpPtr)) {

}
