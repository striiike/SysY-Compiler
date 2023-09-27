//
// Created by hiccup on 2023/9/26.
//

#pragma once

#include <utility>

#include "../define.hpp"

class BlockItem {
private:
    DeclPtr declPtr;
    StmtPtr stmtPtr;
public:
    BlockItem(DeclPtr declPtr, StmtPtr stmtPtr) :
        declPtr(std::move(declPtr)), stmtPtr(std::move(stmtPtr)) {}
};



