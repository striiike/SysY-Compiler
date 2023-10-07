//
// Created by hiccup on 2023/9/27.
//

#pragma once

#include <utility>
#include <vector>
#include "ASTNode.hpp"

class CompUnit : public ASTNode {
    std::vector<DeclPtr> declPtrs;
    std::vector<FuncDefPtr> funcDefPtrs;
    MainFuncDefPtr mainFuncDefPtr;
public:
    explicit CompUnit(std::vector<DeclPtr> declPtrs, std::vector<FuncDefPtr> funcDefPtrs,
                      MainFuncDefPtr mainFuncDefPtr) :
            declPtrs(std::move(declPtrs)),
            funcDefPtrs(std::move(funcDefPtrs)),
            mainFuncDefPtr(std::move(mainFuncDefPtr)) {
        name = "<CompUnit>";
        print();
    }

    void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override {
        symbol.startScope();
        for (const auto &i: declPtrs)
            i->checkError(ctx, ret);
        for (const auto &i: funcDefPtrs)
            i->checkError(ctx, ret);
        mainFuncDefPtr->checkError(ctx, ret);
        symbol.endScope();
    }


};



