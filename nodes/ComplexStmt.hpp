//
// Created by hiccup on 2023/9/26.
//

#pragma once


#include <utility>
#include <vector>

#include "ASTNode.hpp"
#include "Stmt.hpp"
#include "BlockItem.hpp"

class ComplexStmt : public ASTNode {
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

    void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override {
        condPtr->checkError(ctx, ret);
        stmtPtr->checkError(ctx, ret);
    }
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

class Block : public ComplexStmt {
private:
    std::vector<BlockItemPtr> blockItemPtrs;
public:
    explicit Block(std::vector<BlockItemPtr> blockItemPtrs) :
            blockItemPtrs(std::move(blockItemPtrs)) {
        name = "<Block>";
        print();
    }

    void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override {
        symbol.startScope();

    }
};