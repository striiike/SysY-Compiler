//
// Created by hiccup on 2023/9/20.
//

#ifndef COMPILER_TOKEN_ANALYZER_H
#define COMPILER_TOKEN_ANALYZER_H


#include "../lexer/Lexer.h"
#include "lib.hpp"
#include "../Config.hpp"
#include "../nodes/TokenNode.hpp"
#include "Exception.hpp"

#include <bits/stdc++.h>
using namespace std;

class TokenStream {
    std::vector<Token>::iterator current;
    std::vector<Token>::iterator end;
public:
    static std::string tokenTypeToString(TokenType type) {
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
        return tokenTypeStrings[type];
    }

    explicit TokenStream(std::vector<Token> &tokens) : current(tokens.begin()), end(tokens.end()) {}

    std::optional<Token> next() {
        if (current != end) {
            auto token = *(current++);
            if (PARSER_DISPLAY && PARSER_SWITCH)
                outfile << tokenTypeToString(token.type) << " " << token.value << std::endl;
            return token;
        }
        return {};
    }

    std::optional<Token> peek(int n = 0) {
        auto temp = current;
        std::advance(temp, n);
        if (temp != end) {
            return *temp;
        }
        return {};
    }

    bool reachEnd() {
        return current == end;
    }

    TokenNode check(TokenType type, Exception exception) {
        if (peek()->type != type) {
            error(exception);
            /// \useless
            return TokenNode(peek().value());
        } else {
            return TokenNode(next().value());
        }
    }

    void error(Exception exception) {
        errorList.emplace_back(exception, current->lineNum);
    }

};

#endif //COMPILER_TOKEN_ANALYZER_H
