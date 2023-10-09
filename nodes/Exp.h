//
// Created by hiccup on 2023/9/22.
//

#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "TokenNode.h"
#include "ExpUniform.h"


class Number : public ASTNode {
    int num;
public:
    explicit Number(std::string num);

    void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
};

class Exp : public ASTNode {
    bool isConst;
public:
    AddExpPtr addExpPtr;

    explicit Exp(AddExpPtr sharedPtr, bool isConst);

    void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
};

class Cond : public ASTNode {
    LOrExpPtr lOrExpPtr;
public:
    explicit Cond(LOrExpPtr lOrExpPtr);

    void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
};





