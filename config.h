//
// Created by hiccup on 2023/9/20.
//

#pragma once
#include <map>
enum class Exception;
extern bool PARSER_DISPLAY;
extern bool PARSER_SWITCH;

extern std::ofstream outfile;
extern std::ofstream errfile;

extern void parseLog(const std::string &str);

extern std::map<Exception, char> exceptionToString;


