//
// Created by hiccup on 2023/9/20.
//

#ifndef COMPILER_CONFIG_H
#define COMPILER_CONFIG_H


#include <string>

class Config {
public:
    std::string sourceFilePath;
    bool LEXER_DISPLAY;
    bool PARSER_DISPLAY;

    explicit Config();

};


#endif //COMPILER_CONFIG_H
