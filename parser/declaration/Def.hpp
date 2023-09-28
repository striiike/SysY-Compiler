//
// Created by hiccup on 2023/9/25.
//

#ifndef COMPILER_DEF_HPP
#define COMPILER_DEF_HPP


#include "../TokenNode.hpp"
#include "../define.hpp"

class Def : public ASTNode {
private:
    bool isConst;
    TokenNode ident;
    std::vector<ConstExpPtr> constExpPtrs;
    InitValPtr initValPtr;
public:
    Def(bool isConst, TokenNode ident, std::vector<ConstExpPtr> constExpPtrs, InitValPtr initValPtr)
            : isConst(isConst), ident(std::move(ident)), constExpPtrs(std::move(constExpPtrs)),
              initValPtr(std::move(initValPtr)) {
        name = isConst ? "<ConstDef>" : "<VarDef>";
        printInformation();
    }

};


#endif //COMPILER_DEF_HPP
