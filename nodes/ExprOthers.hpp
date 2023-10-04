//
// Created by hiccup on 2023/9/22.
//

#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "TokenNode.hpp"
#include "ExprUniform.hpp"


class Number : public ASTNode {
    int num;
public:
    explicit Number(std::string num) :
            num(std::stoi(num)) {
        name = "<Number>";
        print();
    }
};

class Exp : public ASTNode {
    bool isConst;
public:
    AddExpPtr addExpPtr;

    explicit Exp(AddExpPtr sharedPtr, bool isConst) :
            addExpPtr(std::move(sharedPtr)), isConst(isConst) {
        name = isConst ? "<ConstExp>" : "<Exp>";
        print();
    }

    void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override {
        addExpPtr->checkError(ctx, ret);
    }
};

class LVal : public ASTNode {
    TokenNode ident;
    std::vector<ExpPtr> array;
public:
    LVal(TokenNode ident, std::vector<ExpPtr> array) : ident(std::move(ident)), array(std::move(array)) {
        name = "<LVal>";
        print();
    }

    void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override {

    }

};

class UnaryExp : public ASTNode {
    std::vector<TokenType> unaryOps;
    PrimaryExpPtr primaryExpPtr;
    FunctionCallPtr functionCallPtr;
public:
    UnaryExp(std::vector<TokenType> unaryOps, PrimaryExpPtr primaryExpPtr, FunctionCallPtr functionCallPtr)
            : unaryOps(std::move(unaryOps)), primaryExpPtr(std::move(primaryExpPtr)),
              functionCallPtr(std::move(functionCallPtr)) {
        name = "<UnaryExp>";
        print();
    }

    // Getter methods remain unchanged
    [[nodiscard]] const std::vector<TokenType> &getUnaryOps() const {
        return unaryOps;
    }

    [[nodiscard]] const PrimaryExpPtr &getPrimaryExpPtr() const {
        return primaryExpPtr;
    }

    [[nodiscard]] const FunctionCallPtr &getFunctionCallPtr() const {
        return functionCallPtr;
    }
};

class PrimaryExp : public ASTNode {
    ExpPtr expPtr = nullptr;
    LValPtr lValPtr = nullptr;
    NumberPtr numberPtr = nullptr;
public:
    PrimaryExp(ExpPtr exp, LValPtr lVal, NumberPtr number)
            : expPtr(std::move(exp)), lValPtr(std::move(lVal)),
              numberPtr(std::move(number)) {
        name = "<PrimaryExp>";
        print();
    }

    // Getter methods remain unchanged
    ExpPtr getExpPtr() { return expPtr; }

    LValPtr getLValPtr() { return lValPtr; }

    NumberPtr getNumberPtr() { return numberPtr; }
};

class FuncRParams : public ASTNode {
    std::vector<ExpPtr> expPtrs;
public:
    explicit FuncRParams(std::vector<ExpPtr> expPtrs) : expPtrs(std::move(expPtrs)) {
        name = "<FuncRParams>";
        print();
    }
};

class FunctionCall : public ASTNode {
    std::string ident;
    FuncRParamsPtr funcRParamsPtr;
public:
    FunctionCall(std::string ident, FuncRParamsPtr funcRParamsPtr)
            : ident(std::move(ident)), funcRParamsPtr(std::move(funcRParamsPtr)) {
        name = "<FunctionCall>";
    }
};

class ConstExp : public ASTNode {
    AddExpPtr addExpPtr;
public:
    explicit ConstExp(AddExpPtr sharedPtr) : addExpPtr(std::move(sharedPtr)) {
        name = "<ConstExp>";
        print();
    }
};

class Cond : public ASTNode {
    LOrExpPtr lOrExpPtr;
public:
    explicit Cond(LOrExpPtr lOrExpPtr) : lOrExpPtr(std::move(lOrExpPtr)) {
        name = "<Cond>";
        print();
    }
};

