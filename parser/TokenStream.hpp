//
// Created by hiccup on 2023/9/20.
//

#ifndef COMPILER_TOKEN_ANALYZER_H
#define COMPILER_TOKEN_ANALYZER_H


#include "../lexer/Lexer.h"

#include <optional>

class TokenStream {
private:
    std::vector<Token>::iterator current;
    std::vector<Token>::iterator end;

    /// only used by Parser
    friend class Parser;

public:
    explicit TokenStream(std::vector<Token> &tokens);

    bool reachEnd() const;

    std::optional<Token> next();
    std::optional<Token> peek(int n = 0);
};

#endif //COMPILER_TOKEN_ANALYZER_H
