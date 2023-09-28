//
// Created by hiccup on 2023/9/26.
//

#pragma once

#include <utility>

#include "../define.hpp"
#include "../ASTNode.hpp"

class BlockItem : public ASTNode {
private:
    DeclPtr declPtr;
    StmtPtr stmtPtr;
public:
    BlockItem(DeclPtr declPtr, StmtPtr stmtPtr) :
        declPtr(std::move(declPtr)), stmtPtr(std::move(stmtPtr)) {
        name = "<BlockItem>";
//        printInformation();
    }
};



