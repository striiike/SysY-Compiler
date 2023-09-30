//
// Created by hiccup on 2023/9/30.
//

#pragma once
#include <bits/stdc++.h>
#include "Exception.hpp"

using namespace std;

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

map<Exception, char> exceptionToString = {
        {Exception::INVALID_CHARACTER, 'a'},
        {Exception::REDEFINED_IDENT, 'b'},
        {Exception::UNDEFINED_IDENT, 'c'},
        {Exception::PARAMS_NUM_UNMATCHED, 'd'},
        {Exception::PARAM_TYPE_UNMATCHED, 'e'},
        {Exception::VOID_RETURN_INT, 'f'},
        {Exception::INT_RETURN_LACKED, 'g'},
        {Exception::CONST_ASSIGNED, 'h'},
        {Exception::SEMICN_LACKED, 'i'},
        {Exception::RPARENT_LACKED, 'j'},
        {Exception::RBRACK_LACKED, 'k'},
        {Exception::FORMAT_CHAR_UNMATCHED, 'l'},
        {Exception::BREAK_CONTINUE_OUT_LOOP, 'm'},
        {Exception::UNDEFINED_ERROR, 'n'}
};

vector<pair<Exception, int>> errorList;