//
// Created by hiccup on 2023/9/20.
//

#include <iostream>
#include "Driver.h"
#include "../parser/Parser.h"
#include "../parser/expression/ExpressionParser.hpp"
#include "../parser/expression/other/other.hpp"
#include "../parser/declaration/DeclarationParser.hpp"
#include "../parser/statement/StatementParser.hpp"

void Driver::runCompiler() {

    std::ifstream infile("testfile.txt");
    std::string code((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());

    Lexer lexer(code);
    std::vector<Token> tokens = lexer.analyze();
    lexerDisplay(tokens);

//    auto a = tokens.begin();
//    auto b = tokens.end();
//    for (; a != b; a++){
//        std::cout << (*a).type << std::endl;
//    }
        ;
    TokenStream tokenStream(tokens);


    std::string str = "a?";
    FunctionCall functionCall(str, nullptr);

    /// IT SEEMS NO PROBLEM CURRENTLY
    ExpressionParser expressionParser(tokenStream);

    StatementParser statementParser(tokenStream);
    Parser parser(tokenStream);

    auto _tree = parser.parseCompUnit();

    auto _this = statementParser.parseStmt();



    DeclarationParser declarationParser(tokenStream);
//    auto this_ = declarationParser.parseDecl();

    auto that  = expressionParser.parseLOrExp();
    auto _1 = that->getLeftOperand();
    auto _2 = _1->getLeftOperand();


    parserDisplay();
}


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

void Driver::lexerDisplay(const std::vector<Token> &tokens) {
    std::ofstream outfile("output.txt");
    for (const auto &token: tokens) {
        outfile << tokenTypeToString(token.type) << " " << token.value << std::endl;
    }
    outfile.close();
}

void Driver::parserDisplay() {

}
