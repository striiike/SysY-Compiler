//
// Created by hiccup on 2023/9/21.
//

#pragma once


#include <vector>
#include <memory>
#include "../lexer/Lexer.h"
#include "ASTNode.hpp"
#include "lib.hpp"


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
public:
    ExprUniform(T leftOperand, std::vector<TokenType> operators, std::vector<T> operands)
            : leftOperand(std::move(leftOperand)),
              operators(std::move(operators)),
              operands(std::move(operands)) {}

    T getLeftOperand() {
        return leftOperand;
    };
private:
    T leftOperand;
    std::vector<TokenType> operators;
    std::vector<T> operands;
};


class MulExp : public ExprUniform<std::shared_ptr<UnaryExp>> {
public:
    MulExp(std::shared_ptr<UnaryExp> leftOperand, std::vector<TokenType> operators,
           std::vector<std::shared_ptr<UnaryExp>> operands)
            : ExprUniform<std::shared_ptr<UnaryExp>>(std::move(leftOperand), std::move(operators), std::move(operands)) {
        name = "<MulExp>";
        print();
    }
};


class AddExp : public ExprUniform<std::shared_ptr<MulExp>> {
public:
    AddExp(std::shared_ptr<MulExp> leftOperand, std::vector<TokenType> operators,
           std::vector<std::shared_ptr<MulExp>> operands)
            : ExprUniform<std::shared_ptr<MulExp>>(std::move(leftOperand), std::move(operators), std::move(operands)) {
        name = "<AddExp>";
        print();
    }
};

class RelExp : public ExprUniform<std::shared_ptr<AddExp>> {
public:
    RelExp(std::shared_ptr<AddExp> leftOperand, std::vector<TokenType> operators,
           std::vector<std::shared_ptr<AddExp>> operands)
            : ExprUniform<std::shared_ptr<AddExp>>(std::move(leftOperand), std::move(operators), std::move(operands)) {
        name = "<RelExp>";
        print();
    }
};

class EqExp : public ExprUniform<std::shared_ptr<RelExp>> {
public:
    EqExp(std::shared_ptr<RelExp> leftOperand, std::vector<TokenType> operators,
          std::vector<std::shared_ptr<RelExp>> operands)
            : ExprUniform<std::shared_ptr<RelExp>>(std::move(leftOperand), std::move(operators), std::move(operands)) {
        name = "<EqExp>";
        print();
    }
};

class LAndExp : public ExprUniform<std::shared_ptr<EqExp>> {
public:
    LAndExp(std::shared_ptr<EqExp> leftOperand, std::vector<TokenType> operators,
            std::vector<std::shared_ptr<EqExp>> operands)
            : ExprUniform<std::shared_ptr<EqExp>>(std::move(leftOperand), std::move(operators), std::move(operands)) {
        name = "<LAndExp>";
        print();
    }
};

class LOrExp : public ExprUniform<std::shared_ptr<LAndExp>> {
public:
    LOrExp(std::shared_ptr<LAndExp> leftOperand, std::vector<TokenType> operators,
           std::vector<std::shared_ptr<LAndExp>> operands)
            : ExprUniform<std::shared_ptr<LAndExp>>(std::move(leftOperand), std::move(operators), std::move(operands)) {
        name = "<LOrExp>";
        print();
    }
};
