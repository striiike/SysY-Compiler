//
// Created by hiccup on 2023/9/25.
//

#ifndef COMPILER_DEF_HPP
#define COMPILER_DEF_HPP


#include "TokenNode.hpp"
#include "ExprOthers.hpp"
#include "InitVal.hpp"
#include "../parser/Symbol.hpp"

class Def : public ASTNode {
    bool isConst;
    TokenNode ident;
    std::vector<ExpPtr> expPtrs;
    InitValPtr initValPtr;
public:
    Def(bool isConst, TokenNode ident, std::vector<ExpPtr> expPtrs, InitValPtr initValPtr)
            : isConst(isConst), ident(std::move(ident)), expPtrs(std::move(expPtrs)),
              initValPtr(std::move(initValPtr)) {
        name = isConst ? "<ConstDef>" : "<VarDef>";
        print();
    }

    void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override {
        for (auto &i: expPtrs) {
            i->checkError(ctx, nullptr);
        }

        if (initValPtr) initValPtr->checkError(ctx, ret);
        // no detail
        auto lens = vector<int>(expPtrs.size());
        if (!symbol.insertVar(isConst, ident.getValue(), lens)) {
            errorList.emplace_back(Exception::REDEFINED_IDENT, ident.getLineNum());
        }
    }


};

#endif //COMPILER_DEF_HPP
