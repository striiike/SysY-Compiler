//
// Created by hiccup on 2023/9/21.
//

#pragma once


#include <vector>
#include <memory>
#include "../lexer/Lexer.h"
#include "ASTNode.hpp"

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
class ExprUniform : public ASTNode {
    T leftOperand;
    vector<TokenType> operators;
    vector<T> operands;
public:
    ExprUniform(T leftOperand, vector<TokenType> operators, vector<T> operands)
            : leftOperand(std::move(leftOperand)),
              operators(std::move(operators)),
              operands(std::move(operands)) {}

    T getLeftOperand() {
        return leftOperand;
    };


};


class MulExp : public ExprUniform<UnaryExpPtr> {
public:
    MulExp(UnaryExpPtr leftOperand, vector<TokenType> operators,
           vector<UnaryExpPtr> operands)
            : ExprUniform<UnaryExpPtr>(std::move(leftOperand), std::move(operators), std::move(operands)) {
        name = "<MulExp>";
        print();
    }
};


class AddExp : public ExprUniform<MulExpPtr> {
public:
    AddExp(MulExpPtr leftOperand, vector<TokenType> operators,
           vector<MulExpPtr> operands)
            : ExprUniform<MulExpPtr>(std::move(leftOperand), std::move(operators), std::move(operands)) {
        name = "<AddExp>";
        print();
    }
};

class RelExp : public ExprUniform<AddExpPtr> {
public:
    RelExp(AddExpPtr leftOperand, vector<TokenType> operators,
           vector<AddExpPtr> operands)
            : ExprUniform<AddExpPtr>(std::move(leftOperand), std::move(operators), std::move(operands)) {
        name = "<RelExp>";
        print();
    }
};

class EqExp : public ExprUniform<RelExpPtr> {
public:
    EqExp(RelExpPtr leftOperand, vector<TokenType> operators, vector<RelExpPtr> operands)
            : ExprUniform<RelExpPtr>(std::move(leftOperand), std::move(operators), std::move(operands)) {
        name = "<EqExp>";
        print();
    }
};



class LAndExp : public ExprUniform<EqExpPtr> {
public:
    LAndExp(EqExpPtr leftOperand, vector<TokenType> operators,
            vector<EqExpPtr> operands)
            : ExprUniform<EqExpPtr>(std::move(leftOperand), std::move(operators), std::move(operands)) {
        name = "<LAndExp>";
        print();
    }
};

class LOrExp : public ExprUniform<LAndExpPtr> {
public:
    LOrExp(LAndExpPtr leftOperand, vector<TokenType> operators,
           vector<LAndExpPtr> operands)
            : ExprUniform<LAndExpPtr>(std::move(leftOperand), std::move(operators), std::move(operands)) {
        name = "<LOrExp>";
        print();
    }
};
