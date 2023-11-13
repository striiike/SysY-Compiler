//
// Created by hiccup on 2023/9/22.
//

#pragma once

#include "ASTNode.h"
#include "../lexer/Lexer.h"
#include <utility>
#include <vector>
#include <string>

class TokenNode : public ASTNode {
	std::string value;
	int lineNum;
	TokenType type;
public:
	explicit TokenNode(Token token) :
		value(std::move(token.value)), type(token.type), lineNum(token.lineNum) {}

	TokenType getType() {
		return type;
	}

	string getValue() {
		return value;
	}

	int getLineNum() {
		return lineNum;
	}
};


