//
// Created by hiccup on 2023/9/20.
//

#pragma once
#include <bits/stdc++.h>
#include "parser/Exception.hpp"

extern bool PARSER_DISPLAY;
extern bool PARSER_SWITCH;

extern std::ofstream outfile;

extern void parseLog(const std::string& str);

extern map<Exception, char> exceptionToString;


