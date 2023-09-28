//
// Created by hiccup on 2023/9/22.
//

#ifndef COMPILER_OTHER_HPP
#define COMPILER_OTHER_HPP

#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "../../define.hpp"
#include "../../TokenNode.hpp"


class Number : public ASTNode {
private:
    int num;
public:
    explicit Number(std::string num);
};

class Exp : public ASTNode {
public:
    AddExpPtr addExpPtr;

    explicit Exp(AddExpPtr sharedPtr);
};

class LVal : public ASTNode {
private:
    TokenNode ident;
    std::vector<ExpPtr> array;
public:
    LVal(TokenNode ident, std::vector<ExpPtr> array);
};

class UnaryExp : public ASTNode {
private:
//public:
    std::vector<TokenType> unaryOps;
    PrimaryExpPtr primaryExpPtr;
    FunctionCallPtr functionCallPtr;
public:
    UnaryExp(std::vector<TokenType> unaryOps, PrimaryExpPtr primaryExpPtr, FunctionCallPtr functionCallPtr);

    // Getter methods
    [[nodiscard]] const std::vector<TokenType> &getUnaryOps() const {
        return unaryOps;
    }

    const PrimaryExpPtr &getPrimaryExpPtr() const {
        return primaryExpPtr;
    }

    const FunctionCallPtr &getFunctionCallPtr() const {
        return functionCallPtr;
    }
};

class PrimaryExp : public ASTNode {
private:
    ExpPtr expPtr = nullptr;
    LValPtr lValPtr = nullptr;
    NumberPtr numberPtr = nullptr;
public:
    PrimaryExp(ExpPtr exp, LValPtr lVal, NumberPtr number)
            : expPtr(std::move(exp)), lValPtr(std::move(lVal)),
              numberPtr(std::move(number)) {
        name = "<PrimaryExp>";
        printInformation();
    }

    ExpPtr getExpPtr() { return expPtr; }

    LValPtr getLValPtr() { return lValPtr; }

    NumberPtr getNumberPtr() { return numberPtr; }
};

class FuncRParams : public ASTNode {
private:
    std::vector<ExpPtr> expPtrs;
public:
    explicit FuncRParams(std::vector<ExpPtr> expPtrs);

};

class FunctionCall : public ASTNode {
private:
    std::string ident;
    FuncRParamsPtr funcRParamsPtr;
public:
    FunctionCall(std::string ident, FuncRParamsPtr funcRParamsPtr);;
};

class ConstExp : public ASTNode {
private:
    AddExpPtr addExpPtr;
public:
    explicit ConstExp(AddExpPtr sharedPtr);
};

class Cond : public ASTNode {
private:
    LOrExpPtr lOrExpPtr;
public:
    explicit Cond(LOrExpPtr lOrExpPtr);
};

#endif //COMPILER_OTHER_HPP
