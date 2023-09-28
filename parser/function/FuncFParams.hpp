#include <utility>

//
// Created by hiccup on 2023/9/27.
//

#ifndef COMPILER_FUNCFPARAMS_HPP
#define COMPILER_FUNCFPARAMS_HPP

#include "../define.hpp"
#include "../ASTNode.hpp"

class FuncFParams : public ASTNode{
private:
    std::vector<FuncFParamPtr> funcFParamPtrs;
public:
    explicit FuncFParams(std::vector<FuncFParamPtr> funcFParamPtrs) :
            funcFParamPtrs(std::move(funcFParamPtrs)) {
        name = "<FuncFParams>";
        printInformation();
    }
};


#endif //COMPILER_FUNCFPARAMS_HPP
