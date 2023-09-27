//
// Created by hiccup on 2023/9/25.
//

#ifndef COMPILER_DEF_HPP
#define COMPILER_DEF_HPP


#include "../TokenNode.hpp"
#include "../define.hpp"

class Def {
private:
    TokenNode ident;
    std::vector<ConstExpPtr> constExpPtrs;
    InitValPtr initValPtr;
public:
    Def(TokenNode ident, std::vector<ConstExpPtr> constExpPtrs, InitValPtr initValPtr);
};


#endif //COMPILER_DEF_HPP
