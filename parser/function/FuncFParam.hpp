//
// Created by hiccup on 2023/9/27.
//

#ifndef COMPILER_FUNCFPARAM_HPP
#define COMPILER_FUNCFPARAM_HPP

#include "../define.hpp"
#include "../ASTNode.hpp"

class FuncFParam : public ASTNode {
private:
    TokenNode bType;
    TokenNode ident;
    bool isArray;
    std::vector<ConstExpPtr> constExpPtrs;
public:
    FuncFParam(TokenNode bType, TokenNode ident, bool isArray, std::vector<ConstExpPtr> constExpPtrs)
            : bType(std::move(bType)),
              ident(std::move(ident)),
              isArray(isArray),
              constExpPtrs(std::move(constExpPtrs)) {
        name = "<FuncFParam>";
        printInformation();
    }
};


#endif //COMPILER_FUNCFPARAM_HPP
