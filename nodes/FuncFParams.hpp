//
// Created by hiccup on 2023/9/27.
//

#pragma once

#include <utility>
#include "ASTNode.hpp"

class FuncFParams : public ASTNode{
private:
    std::vector<FuncFParamPtr> funcFParamPtrs;
public:
    explicit FuncFParams(std::vector<FuncFParamPtr> funcFParamPtrs) :
            funcFParamPtrs(std::move(funcFParamPtrs)) {
        name = "<FuncFParams>";
        print();
    }
};



