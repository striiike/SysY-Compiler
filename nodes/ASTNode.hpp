//
// Created by hiccup on 2023/9/21.
//

#pragma once

#include <iostream>
#include "../Config.hpp"

class ASTNode {
public:
    std::string name;

    void print() const {
        if (PARSER_DISPLAY && PARSER_SWITCH)
            outfile << name << std::endl;
    }

};


