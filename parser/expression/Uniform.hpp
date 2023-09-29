//
// Created by hiccup on 2023/9/21.
//

#pragma once


#include <vector>
#include <memory>
#include "../../lexer/Lexer.h"
#include "../ASTNode.hpp"
#include "../define.hpp"


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
class Uniform : public ASTNode {
public:
    Uniform(T leftOperand, std::vector<TokenType> operators, std::vector<T> operands)
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


class MulExp : public Uniform<std::shared_ptr<UnaryExp>> {
public:
    MulExp(std::shared_ptr<UnaryExp> leftOperand, std::vector<TokenType> operators,
           std::vector<std::shared_ptr<UnaryExp>> operands)
            : Uniform<std::shared_ptr<UnaryExp>>(std::move(leftOperand), std::move(operators), std::move(operands)) {
        name = "<MulExp>";
        printInformation();
    }
};


class AddExp : public Uniform<std::shared_ptr<MulExp>> {
public:
    AddExp(std::shared_ptr<MulExp> leftOperand, std::vector<TokenType> operators,
           std::vector<std::shared_ptr<MulExp>> operands)
            : Uniform<std::shared_ptr<MulExp>>(std::move(leftOperand), std::move(operators), std::move(operands)) {
        name = "<AddExp>";
        printInformation();
    }
};

class RelExp : public Uniform<std::shared_ptr<AddExp>> {
public:
    RelExp(std::shared_ptr<AddExp> leftOperand, std::vector<TokenType> operators,
           std::vector<std::shared_ptr<AddExp>> operands)
            : Uniform<std::shared_ptr<AddExp>>(std::move(leftOperand), std::move(operators), std::move(operands)) {
        name = "<RelExp>";
        printInformation();
    }
};

class EqExp : public Uniform<std::shared_ptr<RelExp>> {
public:
    EqExp(std::shared_ptr<RelExp> leftOperand, std::vector<TokenType> operators,
          std::vector<std::shared_ptr<RelExp>> operands)
            : Uniform<std::shared_ptr<RelExp>>(std::move(leftOperand), std::move(operators), std::move(operands)) {
        name = "<EqExp>";
        printInformation();
    }
};

class LAndExp : public Uniform<std::shared_ptr<EqExp>> {
public:
    LAndExp(std::shared_ptr<EqExp> leftOperand, std::vector<TokenType> operators,
            std::vector<std::shared_ptr<EqExp>> operands)
            : Uniform<std::shared_ptr<EqExp>>(std::move(leftOperand), std::move(operators), std::move(operands)) {
        name = "<LAndExp>";
        printInformation();
    }
};

class LOrExp : public Uniform<std::shared_ptr<LAndExp>> {
public:
    LOrExp(std::shared_ptr<LAndExp> leftOperand, std::vector<TokenType> operators,
           std::vector<std::shared_ptr<LAndExp>> operands)
            : Uniform<std::shared_ptr<LAndExp>>(std::move(leftOperand), std::move(operators), std::move(operands)) {
        name = "<LOrExp>";
        printInformation();
    }
};
