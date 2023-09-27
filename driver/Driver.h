//
// Created by hiccup on 2023/9/20.
//

#ifndef COMPILER_DRIVER_H
#define COMPILER_DRIVER_H


#include <vector>
#include "../lexer/Lexer.h"

class Driver {
public:
    void runCompiler();

    void parserDisplay();

    static void lexerDisplay(const std::vector<Token>& tokens);
};


#endif //COMPILER_DRIVER_H
