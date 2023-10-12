#include "FuncFParams.h"
#include "FuncFParam.h"
#include "../parser/Symbol.hpp"

FuncFParams::FuncFParams(std::vector<FuncFParamPtr> funcFParamPtrs) :
        funcFParamPtrs(std::move(funcFParamPtrs)) {
    name = "<FuncFParams>";
    print();
}

vector<int> FuncFParams::getDims() {
    vector<int> dims{};
    for (const auto& i : funcFParamPtrs) {
        dims.push_back(i->getDim());
    }
    return dims;
}


void FuncFParams::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
    for (const auto& i : funcFParamPtrs) {
        i->checkError(ctx, ret);
    }
//    symbol.endScope();
}

