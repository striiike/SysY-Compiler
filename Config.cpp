//
// Created by hiccup on 2023/9/20.
//

#include "Config.h"


Config::Config() :
        sourceFilePath("testfile.txt"),
        LEXER_DISPLAY(true),
        PARSER_DISPLAY(true) {}

bool PARSER_DISPLAY = true;
bool PARSER_SWITCH = true;

std::ofstream outfile("output.txt");

void printString(std::string str) {
    if (PARSER_DISPLAY && PARSER_SWITCH) {
        outfile << str << std::endl;    }
}
