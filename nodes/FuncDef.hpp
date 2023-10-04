//
// Created by hiccup on 2023/9/27.
//

#ifndef COMPILER_FUNCDEF_HPP
#define COMPILER_FUNCDEF_HPP


#include <utility>

#include "TokenNode.hpp"
#include "FuncFParams.hpp"
#include "ComplexStmt.hpp"

class FuncDef : public ASTNode {
private:
    TokenNode funcType;
    TokenNode ident;
    FuncFParamsPtr funcFParamsPtr;
    BlockPtr blockPtr;
public:
    FuncDef(TokenNode funcType, TokenNode ident, FuncFParamsPtr funcFParamsPtr, BlockPtr blockPtr)
            : funcType(std::move(funcType)), ident(std::move(ident)),
              funcFParamsPtr(std::move(funcFParamsPtr)),
              blockPtr(std::move(blockPtr)) {
        name = "<FuncDef>";
        print();
    }

    void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override {
        if (funcFParamsPtr) funcFParamsPtr->checkError(ctx, ret);
        blockPtr->checkError(ctx, ret);

        // no detail for check error
        if (!symbol.insertFunc(ident.getValue(), funcType.getType() == VOIDTK)) {
            errorList.emplace_back(Exception::REDEFINED_IDENT, ident.getLineNum());
        }
    }
};

#endif //COMPILER_FUNCDEF_HPP

