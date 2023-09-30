//
// Created by hiccup on 2023/9/18.
//
#include <bits/stdc++.h>
#include "parser/Parser.hpp"
#include "Config.h"

bool PARSER_DISPLAY = true;
bool PARSER_SWITCH = true;

std::ofstream outfile("output.txt");

void printString(std::string str) {
    if (PARSER_DISPLAY && PARSER_SWITCH) {
        outfile << str << std::endl;
    }
}

int main() {

    std::ifstream infile("testfile.txt");
    std::string code((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());


    Lexer lexer(code);
    std::vector<Token> tokens = lexer.analyze();

    TokenStream tokenStream(tokens);
    Parser parser(tokenStream);

    auto _tree = parser.parseCompUnit();

    return 0;
}