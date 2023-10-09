//
// Created by hiccup on 2023/9/21.
//

#pragma once


#include <vector>
#include <memory>
#include "../lexer/Lexer.h"
#include "ASTNode.h"
#include "UnaryExp.h"

using namespace std;

// only for certain syntax as (MulExp, AddExp, RelExp, EqExp, LAndExp, LOrExp)
//
//    <MulExp>        := <UnaryExp> { ('*' | '/' | '%') <UnaryExp> }
//    <AddExp>        := <MulExp> { ('+' | '-') <MulExp> }
//    <RelExp>        := <AddExp> { ('<' | '>' | '<=' | '>=') <AddExp> }
//    <EqExp>         := <RelExp> { ('==' | '!=') <RelExp> }
//    <LAndExp>       := <EqExp> { '&&' <EqExp> }
//    <LOrExp>        := <LAndExp> { '||' <LAndExp> }
//


template<typename T>
class ExpUniform : public ASTNode {
    T leftOperand;
    vector<TokenType> operators;
    vector<T> operands;
public:
    ExpUniform(T leftOperand, vector<TokenType> operators, vector<T> operands)
            : leftOperand(std::move(leftOperand)),
              operators(std::move(operators)),
              operands(std::move(operands)) {}

    T getLeftOperand() {
        return leftOperand;
    };

    void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override {
        leftOperand->checkError(ctx, ret);
        for (const auto &i: operands) {
            i->checkError(ctx, ret);
        }
    }
};


class MulExp : public ExpUniform<UnaryExpPtr> {
public:
    MulExp(UnaryExpPtr leftOperand, vector<TokenType> operators,
           vector<UnaryExpPtr> operands);
};


class AddExp : public ExpUniform<MulExpPtr> {
public:
    AddExp(MulExpPtr leftOperand, vector<TokenType> operators,
           vector<MulExpPtr> operands);
};

class RelExp : public ExpUniform<AddExpPtr> {
public:
    RelExp(AddExpPtr leftOperand, vector<TokenType> operators,
           vector<AddExpPtr> operands);
};

class EqExp : public ExpUniform<RelExpPtr> {
public:
    EqExp(RelExpPtr leftOperand, vector<TokenType> operators, vector<RelExpPtr> operands);
};


class LAndExp : public ExpUniform<EqExpPtr> {
public:
    LAndExp(EqExpPtr leftOperand, vector<TokenType> operators,
            vector<EqExpPtr> operands);
};

class LOrExp : public ExpUniform<LAndExpPtr> {
public:
    LOrExp(LAndExpPtr leftOperand, vector<TokenType> operators,
           vector<LAndExpPtr> operands);
};
