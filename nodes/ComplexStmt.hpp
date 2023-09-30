//
// Created by hiccup on 2023/9/26.
//

#pragma once


#include <utility>
#include <vector>

#include "lib.hpp"
#include "ASTNode.hpp"

class ComplexStmt {
public:
    virtual void debug() {}
};

class IfStmt : public ComplexStmt {
private:
    CondPtr condPtr;
    StmtPtr stmtPtr;
    StmtPtr stmtElsePtr;
public :
    IfStmt(CondPtr condPtr, StmtPtr stmtPtr, StmtPtr stmtElsePtr) :
            condPtr(std::move(condPtr)), stmtPtr(std::move(stmtPtr)), stmtElsePtr(std::move(stmtElsePtr)) {}
};

class ForStmt : public ComplexStmt {
private:
    _ForStmtPtr assignStmtPtr1;
    _ForStmtPtr assignStmtPtr2;
    CondPtr condPtr;
    StmtPtr stmtPtr;
public :
    ForStmt(CondPtr condPtr, StmtPtr stmtPtr,
            _ForStmtPtr assignStmtPtr1, _ForStmtPtr assignStmtPtr2) :
            condPtr(std::move(condPtr)), stmtPtr(std::move(stmtPtr)),
            assignStmtPtr1(std::move(assignStmtPtr1)),
            assignStmtPtr2(std::move(assignStmtPtr2)) {}
};

class Block : public ComplexStmt, ASTNode {
private:
    std::vector<BlockItemPtr> blockItemPtrs;
public:
    explicit Block(std::vector<BlockItemPtr> blockItemPtrs) :
            blockItemPtrs(std::move(blockItemPtrs)) {
        name = "<Block>";
        print();
    }
};