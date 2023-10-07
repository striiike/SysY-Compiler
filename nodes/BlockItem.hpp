//
// Created by hiccup on 2023/9/26.
//

#pragma once

#include <utility>

#include "ASTNode.hpp"
#include "Decl.hpp"
#include "Stmt.hpp"

class BlockItem : public ASTNode {
private:
    DeclPtr declPtr;
    StmtPtr stmtPtr;
public:
    BlockItem(DeclPtr declPtr, StmtPtr stmtPtr) :
        declPtr(std::move(declPtr)), 
        stmtPtr(std::move(stmtPtr)) {
        name = "<BlockItem>";
    }

    void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override {
        if (declPtr) declPtr->checkError(ctx, ret);
        if (stmtPtr) stmtPtr->checkError(ctx, ret);
    }


};



