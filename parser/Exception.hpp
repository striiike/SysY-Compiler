//
// Created by hiccup on 2023/9/30.
//

#pragma once
#include <bits/stdc++.h>
using namespace std;

enum class Exception {
    INVALID_CHARACTER,  //
    REDEFINED_IDENT,
    UNDEFINED_IDENT,
    PARAMS_NUM_UNMATCHED,
    PARAM_TYPE_UNMATCHED,
    VOID_RETURN_INT,
    INT_RETURN_LACKED,
    CONST_ASSIGNED,
    SEMICN_LACKED,  // #
    RPARENT_LACKED, // #
    RBRACK_LACKED,  // #
    FORMAT_CHAR_UNMATCHED,
    BREAK_CONTINUE_OUT_LOOP,
    UNDEFINED_ERROR,
    CORRECT
};

inline map<Exception, char> exceptionToString = {
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

inline vector<pair<Exception, int>> errorList;