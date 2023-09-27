//
// Created by hiccup on 2023/9/26.
//

#ifndef COMPILER_STMT_HPP
#define COMPILER_STMT_HPP


#include <utility>

#include "../define.hpp"
#include "SimpleStmt.hpp"
#include "ComplexStmt.hpp"

class Stmt {
private:
    SimpleStmtPtr simpleStmtPtr;
    ComplexStmtPtr complexStmtPtr;
public:
    Stmt(SimpleStmtPtr simpleStmt, ComplexStmtPtr complexStmt) :
            simpleStmtPtr(std::move(simpleStmt)), complexStmtPtr(std::move(complexStmt)){}
};


#endif //COMPILER_STMT_HPP
