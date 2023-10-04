//
// Created by hiccup on 2023/9/26.
//

#pragma once

#include <utility>

#include "ASTNode.hpp"







class Stmt : public ASTNode {
private:
    SimpleStmtPtr simpleStmtPtr;
    ComplexStmtPtr complexStmtPtr;
public:
    Stmt(SimpleStmtPtr simpleStmt, ComplexStmtPtr complexStmt) :
            simpleStmtPtr(std::move(simpleStmt)), complexStmtPtr(std::move(complexStmt)){
        name = "<Stmt>";
        print();
    }
};
