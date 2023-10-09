#include "FuncFParam.h"
#include "Exp.h"

FuncFParam::FuncFParam(TokenNode bType, TokenNode ident, bool isArray, std::vector<ExpPtr> expPtrs)
        : bType(std::move(bType)),
          ident(std::move(ident)),
          isArray(isArray),
          expPtrs(std::move(expPtrs)) {
    name = "<FuncFParam>";
    print();
}

void FuncFParam::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
    for (const auto& i : expPtrs) {
        i->checkError(ctx, ret);
    }
}
