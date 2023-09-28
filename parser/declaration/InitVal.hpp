//
// Created by hiccup on 2023/9/25.
//

#pragma once

#include <utility>
#include <vector>
#include "../define.hpp"
#include "../ASTNode.hpp"


class InitVal : public ASTNode {
public:
    virtual void debug() {}
};

class ExpInitVal : public InitVal {
private:
    bool isConst;
    ExpPtr expPtr;
    ConstExpPtr constExpPtr;
public:
    ExpInitVal(bool isConst, ExpPtr expPtr, ConstExpPtr constExpPtr) :
            isConst(isConst), expPtr(std::move(expPtr)), constExpPtr(std::move(constExpPtr)) {
        name = isConst ? "<ConstInitVal>" : "<InitVal>";
        printInformation();
    }
};

class ArrayInitVal : public InitVal {
private:
    bool isConst;
    std::vector<InitValPtr> initValPtrs;
public:
    explicit ArrayInitVal(bool isConst, std::vector<InitValPtr> initValPtrs) :
            isConst(isConst), initValPtrs(std::move(initValPtrs)) {
        name = isConst ? "<ConstInitVal>" : "<InitVal>";
        printInformation();
    }
};

