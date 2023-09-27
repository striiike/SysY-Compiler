//
// Created by hiccup on 2023/9/25.
//

#include "Def.hpp"

#include <utility>

Def::Def(TokenNode ident, std::vector<ConstExpPtr> constExpPtrs, InitValPtr initValPtr)
        : ident(std::move(ident)), constExpPtrs(std::move(constExpPtrs)), initValPtr(std::move(initValPtr)) {
}
