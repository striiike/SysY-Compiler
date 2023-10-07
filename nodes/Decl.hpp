//
// Created by hiccup on 2023/9/25.
//

#pragma once


#include <utility>
#include <vector>
#include "ASTNode.hpp"
#include "TokenNode.hpp"
#include "Def.hpp"

class Decl : public ASTNode {
    bool isConst;
    TokenNode bType;
    std::vector<DefPtr> defPtrs;
public :
    Decl(bool isConst, TokenNode bType, std::vector<DefPtr> defPtrs) :
            isConst(isConst),
            bType(std::move(bType)),
            defPtrs(std::move(defPtrs)) {
        name = isConst ? "<ConstDecl>" : "<VarDecl>";
        print();
    }

    void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override {
        ctx->isConst = this->isConst;
        for (const auto &i: defPtrs)
            i->checkError(ctx, ret);
        ctx->isConst = false;
    }
};


