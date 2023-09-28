//
// Created by hiccup on 2023/9/21.
//

#pragma once

#include <iostream>
#include "../driver/Config.h"

class ASTNode {
public:
    std::string name;

    void printInformation() const {
        if (PARSER_DISPLAY && PARSER_SWITCH)
            outfile << name << std::endl;
    }
};


