#include "FuncFParams.h"
#include "FuncFParam.h"
#include "../parser/Symbol.hpp"

FuncFParams::FuncFParams(std::vector<FuncFParamPtr> funcFParamPtrs) :
        funcFParamPtrs(std::move(funcFParamPtrs)) {
    name = "<FuncFParams>";
    print();
}

void FuncFParams::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
    symbol.startScope();
    for (const auto& i : funcFParamPtrs) {
        i->checkError(ctx, ret);
    }
    symbol.endScope();
}

