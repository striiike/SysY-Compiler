//
// Created by hiccup on 2023/9/25.
//

#pragma once


#include <utility>
#include <vector>
#include "../parser/TokenNode.hpp"
#include "lib.hpp"

class Decl : public ASTNode{
private:
    bool isConst;
    TokenNode bType;
    std::vector<DefPtr> defPtrs;
public :
    Decl(bool isConst, TokenNode bType, std::vector<DefPtr> defPtrs) :
            isConst(isConst), bType(std::move(bType)), defPtrs(std::move(defPtrs)) {
        name = isConst ? "<ConstDecl>" : "<VarDecl>";
        printInformation();
    }
};


