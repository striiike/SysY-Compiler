//
// Created by hiccup on 2023/9/26.
//

#pragma once

#include <vector>
#include "lib.hpp"
#include "TokenNode.hpp"

class SimpleStmt {
public:
    virtual void debug() {}
};

class AssignStmt : public SimpleStmt {
private:
    LValPtr lValPtr;
    ExpPtr expPtr;
public:
    AssignStmt(LValPtr lValPtr, ExpPtr expPtr)
            : lValPtr(std::move(lValPtr)), expPtr(std::move(expPtr)) {}
};

class _ForStmt : public ASTNode {
private:
    LValPtr lValPtr;
    ExpPtr expPtr;
public:
    _ForStmt(LValPtr lValPtr, ExpPtr expPtr)
            : lValPtr(std::move(lValPtr)), expPtr(std::move(expPtr)) {
        name = "<ForStmt>";
        print();
    }
};

class ExpStmt : public SimpleStmt {
private:
    ExpPtr expPtr;
public:
    explicit ExpStmt(ExpPtr expPtr)
            : expPtr(std::move(expPtr)) {}
};

class BreakStmt : public SimpleStmt {
private:
    TokenNode _break;
public:
    explicit BreakStmt(TokenNode _break)
            : _break(std::move(_break)) {}
};

class ContinueStmt : public SimpleStmt {
private:
    TokenNode _continue;
public:
    explicit ContinueStmt(TokenNode _continue)
            : _continue(std::move(_continue)) {}
};

class ReturnStmt : public SimpleStmt {
private:
    TokenNode _return;
    ExpPtr expPtr;
public:
    ReturnStmt(TokenNode _return, ExpPtr expPtr)
            : _return(std::move(_return)), expPtr(std::move(expPtr)) {}
};

class GetintStmt : public SimpleStmt {
private:
    LValPtr lValPtr;
    TokenNode _getint;
public:
    GetintStmt(LValPtr lValPtr, TokenNode _getint)
            : lValPtr(std::move(lValPtr)), _getint(std::move(_getint)) {}
};

class PrintfStmt : public SimpleStmt {
private:
    TokenNode _printf;
    TokenNode FormatString;
    std::vector<ExpPtr> expPtrs;
public:
    PrintfStmt(TokenNode _printf, TokenNode FormatString, std::vector<ExpPtr> expPtrs)
            : _printf(std::move(_printf)), FormatString(std::move(FormatString)), expPtrs(std::move(expPtrs)) {}
};
