//
// Created by hiccup on 2023/10/9.
//
#pragma once


#include "ASTNode.h"
#include "TokenNode.h"

class LVal : public ASTNode {
    TokenNode ident;
    std::vector<ExpPtr> array;
public:
    LVal(TokenNode ident, std::vector<ExpPtr> array);

    void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
};

