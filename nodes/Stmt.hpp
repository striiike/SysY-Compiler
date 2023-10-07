//
// Created by hiccup on 2023/9/26.
//

#pragma once

#include <utility>

#include "ASTNode.hpp"


class Stmt : public ASTNode {
public:
    Stmt() {
        name = "<Stmt>";
    }
};
