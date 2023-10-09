//
// Created by hiccup on 2023/10/7.
//

#pragma once

#include "ASTNode.h"
#include "TokenNode.h"

class UnaryExp : public ASTNode {
    std::vector<TokenType> unaryOps;
    PrimaryExpPtr primaryExpPtr;
    FunctionCallPtr functionCallPtr;
public:
    UnaryExp(std::vector<TokenType> unaryOps, PrimaryExpPtr primaryExpPtr, FunctionCallPtr functionCallPtr);

    // Getter methods remain unchanged
    [[nodiscard]] const std::vector<TokenType> &getUnaryOps() const;

    [[nodiscard]] const PrimaryExpPtr &getPrimaryExpPtr() const;

    [[nodiscard]] const FunctionCallPtr &getFunctionCallPtr() const;

    void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
};

