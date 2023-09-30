//
// Created by hiccup on 2023/9/19.
//


#pragma once

#include <vector>
#include <string>
#include <fstream>

enum TokenType {
    IDENFR, NOT, MULT, ASSIGN,
    INTCON, AND, DIV, SEMICN,
    STRCON, OR, MOD, COMMA,
    MAINTK, FORTK, LSS, LPARENT,
    CONSTTK, GETINTTK, LEQ, RPARENT,
    INTTK, PRINTFTK, GRE, LBRACK,
    BREAKTK, RETURNTK, GEQ, RBRACK,
    CONTINUETK, PLUS, EQL, LBRACE,
    IFTK, MINU, NEQ, RBRACE,
    ELSETK, VOIDTK,
};

struct Token {
    TokenType type;
    std::string value;
    int lineNum;
};

class Lexer {
public:
    explicit Lexer(const std::string &code);

    std::vector<Token> analyze();

private:
    std::string code;
    std::vector<Token> tokens;
    std::size_t pos;
    std::size_t curLineNum = 0;

    char peek(int offset);

    char next();

    void skipWhitespace();

    void tokenize();

    void tokenizeNumber();

    void tokenizeWord();

    void tokenizeOperatorOrPunctuation();

    void tokenizeString();
};
