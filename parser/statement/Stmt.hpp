//
// Created by hiccup on 2023/9/26.
//

#ifndef COMPILER_STMT_HPP
#define COMPILER_STMT_HPP


#include <utility>

#include "../define.hpp"
#include "../ASTNode.hpp"
#include "SimpleStmt.hpp"
#include "ComplexStmt.hpp"

class Stmt : public ASTNode {
private:
    SimpleStmtPtr simpleStmtPtr;
    ComplexStmtPtr complexStmtPtr;
public:
    Stmt(SimpleStmtPtr simpleStmt, ComplexStmtPtr complexStmt) :
            simpleStmtPtr(std::move(simpleStmt)), complexStmtPtr(std::move(complexStmt)){
        name = "<Stmt>";
        printInformation();
    }
};


#endif //COMPILER_STMT_HPP
