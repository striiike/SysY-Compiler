#include <utility>

//
// Created by hiccup on 2023/9/27.
//
#pragma once

#include "lib.hpp"
#include "ASTNode.hpp"

class MainFuncDef : public ASTNode{
private:
    TokenNode bType;
    TokenNode _main;
    BlockPtr blockPtr;
public:
    MainFuncDef(TokenNode bType, TokenNode _main, BlockPtr blockPtr) :
            bType(std::move(bType)), _main(std::move(_main)), blockPtr(std::move(blockPtr)) {
        name = "<MainFuncDef>";
        print();
    }
};

