//
// Created by hiccup on 2023/9/27.
//

#ifndef COMPILER_FUNCDEF_HPP
#define COMPILER_FUNCDEF_HPP


#include <utility>

#include "../TokenNode.hpp"
#include "../define.hpp"

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
        printInformation();
    }
};

#endif //COMPILER_FUNCDEF_HPP
