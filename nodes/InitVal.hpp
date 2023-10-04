//
// Created by hiccup on 2023/9/25.
//

#pragma once

#include <utility>
#include <vector>
#include "ASTNode.hpp"


class InitVal : public ASTNode {
public:
    virtual void debug() {}
};

class ExpInitVal : public InitVal {
private:
    bool isConst;
    ExpPtr expPtr;
public:
    ExpInitVal(bool isConst, ExpPtr expPtr) :
            isConst(isConst), expPtr(std::move(expPtr)) {
        name = isConst ? "<ConstInitVal>" : "<InitVal>";
        print();
    }

//    void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override {
//        if ()
//    }
};

class ArrayInitVal : public InitVal {
private:
    bool isConst;
    std::vector<InitValPtr> initValPtrs;
public:
    explicit ArrayInitVal(bool isConst, std::vector<InitValPtr> initValPtrs) :
            isConst(isConst), initValPtrs(std::move(initValPtrs)) {
        name = isConst ? "<ConstInitVal>" : "<InitVal>";
        print();
    }

    void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override {

    }
};

