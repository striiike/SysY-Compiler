//
// Created by hiccup on 2023/9/20.
//

#ifndef COMPILER_TOKEN_ANALYZER_H
#define COMPILER_TOKEN_ANALYZER_H


#include "../lexer/Lexer.h"
#include "lib.hpp"
#include "../Config.hpp"

#include <optional>

class TokenStream {
    std::vector<Token>::iterator current;
    std::vector<Token>::iterator end;
public:
    static std::string tokenTypeToString(TokenType type) {
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

    bool reachEnd() const {
        return current == end;
    }

};

#endif //COMPILER_TOKEN_ANALYZER_H
