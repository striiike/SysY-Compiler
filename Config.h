//
// Created by hiccup on 2023/9/20.
//

#pragma once
#include <string>
#include <fstream>

class Config {
public:
    std::string sourceFilePath;
    bool LEXER_DISPLAY;
    bool PARSER_DISPLAY;

    explicit Config();

};

extern bool PARSER_DISPLAY;
extern bool PARSER_SWITCH;

extern std::ofstream outfile;

extern void printString(std::string str);


