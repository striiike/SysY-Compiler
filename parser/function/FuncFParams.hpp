#include <utility>

//
// Created by hiccup on 2023/9/27.
//

#ifndef COMPILER_FUNCFPARAMS_HPP
#define COMPILER_FUNCFPARAMS_HPP


class FuncFParams {
private:
    std::vector<FuncFParamPtr> funcFParamPtrs;
public:
    explicit FuncFParams(std::vector<FuncFParamPtr> funcFParamPtrs) :
            funcFParamPtrs(std::move(funcFParamPtrs)) {}
};


#endif //COMPILER_FUNCFPARAMS_HPP
