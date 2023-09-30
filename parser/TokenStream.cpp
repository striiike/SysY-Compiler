//
// Created by hiccup on 2023/9/20.
//

#include <optional>
#include <iostream>
#include "TokenStream.hpp"
#include "../Config.hpp"

const char *tokenTypeStrings[] = {
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

TokenStream::TokenStream(std::vector<Token> &tokens) : current(tokens.begin()), end(tokens.end()) {}

std::optional<Token> TokenStream::next() {
    if (current != end) {
        auto token = *(current++);
        if (PARSER_DISPLAY && PARSER_SWITCH)
            outfile << tokenTypeToString(token.type) << " " << token.value << std::endl;
        return token;
    }
    return {};
}

std::optional<Token> TokenStream::peek(int n) {
    auto temp = current;
    std::advance(temp, n);
    if (temp != end) {
        return *temp;
    }
    return {};
}

bool TokenStream::reachEnd() const {
    return current == end;
}
