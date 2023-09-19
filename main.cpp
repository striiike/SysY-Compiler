//
// Created by hiccup on 2023/9/18.
//
#include <iostream>
#include <fstream>
#include <string>
#include "Lexer.h"



const char* tokenTypeStrings[] = {
        "IDENFR", "NOT", "MULT", "ASSIGN",
        "INTCON", "AND", "DIV", "SEMICN",
        "STRCON", "OR", "MOD", "COMMA",
        "MAINTK", "FORTK", "LSS", "LPARENT",
        "CONSTTK", "GETINTTK", "LEQ", "RPARENT",
        "INTTK", "PRINTFTK", "GRE", "LBRACK",
        "BREAKTK", "RETURNTK", "GEQ", "RBRACK",
        "CONTINUETK", "PLUS", "EQL", "LBRACE",
        "IFTK", "MINU", "NEQ", "RBRACE",
        "ELSETK", "VOIDTK"
};

std::string tokenTypeToString(TokenType type) {
    return tokenTypeStrings[type];
}

int main() {

    std::ifstream infile("./testfile.txt");
    std::string code((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());

    Lexer lexer(code);
    std::vector<Token> tokens = lexer.analyze();

    std::ofstream outfile("./output.txt");
    for (const auto& token : tokens) {
        outfile << tokenTypeToString(token.type) << " " << token.value << std::endl;
    }
    outfile.close();

    return 0;
}