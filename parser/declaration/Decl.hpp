//
// Created by hiccup on 2023/9/25.
//

#ifndef COMPILER_DECL_HPP
#define COMPILER_DECL_HPP


#include <utility>
#include <vector>
#include "../TokenNode.hpp"

class Decl {
private:
    bool isConst;
    TokenNode bType;
    std::vector<DefPtr> defPtrs;
public :
    Decl(bool isConst, TokenNode bType, std::vector<DefPtr> defPtrs) :
            isConst(isConst), bType(std::move(bType)), defPtrs(std::move(defPtrs)) {}
};


#endif //COMPILER_DECL_HPP
