//
// Created by hiccup on 2023/9/19.
//
#include "Lexer.h"
#include <map>
#include <cctype>
#include <unordered_set>

std::map<std::string, TokenType> keywordMap = {
	{"int", INTTK},
	{"void", VOIDTK},
	{"if", IFTK},
	{"else", ELSETK},
	{"for", FORTK},
	{"break", BREAKTK},
	{"continue", CONTINUETK},
	{"return", RETURNTK},
	{"main", MAINTK},
	{"const", CONSTTK},
	{"getint", GETINTTK},
	{"printf", PRINTFTK}
};

Lexer::Lexer(const std::string &code) : code(code), pos(0) {}

char Lexer::peek(int offset = 0) {
	size_t position = pos + offset;
	return position < code.size() ? code[position] : '\0';
}

char Lexer::next() {
	return pos < code.size() ? code[pos++] : '\0';
}

void Lexer::skipWhitespace() {
	while (std::isspace(peek())) {
		if (peek()=='\n')
			curLineNum++;
		next();
	}
}

void Lexer::tokenize() {
	skipWhitespace();

	// skip single-line comment
	if (peek()=='/' && peek(1)=='/') {
		while (peek() && next()!='\n');
		curLineNum++;
	}

		// skip multi-lines comment
	else if (peek()=='/' && peek(1)=='*') {
		next();
		next();
		while (peek() && (peek()!='*' || peek(1)!='/')) {
			if (peek()=='\n')
				curLineNum++;
			next();
		}
		next();
		next();
	} else if (peek()=='\"') {
		tokenizeString();
	} else if (std::isdigit(peek())) {
		tokenizeNumber();
	} else if (std::isalpha(peek()) || peek()=='_') {
		tokenizeWord();
	} else {
		tokenizeOperatorOrPunctuation();
	}
}

void replaceEnter(std::string &s) {
	std::string target = "\\n";
	std::string replacement = "\n";

	size_t startPos = 0;
	while ((startPos = s.find(target, startPos))!=std::string::npos) {
		s.replace(startPos, target.length(), replacement);
		startPos += replacement.length();
	}
}

void Lexer::tokenizeString() {
	std::string str;
	str.push_back(next());  // Skip the opening quote
	while (peek()!='\"') {
		str.push_back(next());
	}
	str.push_back(next());  // Skip the closing quote
	replaceEnter(str);
	tokens.push_back({STRCON, str, curLineNum});
}

void Lexer::tokenizeNumber() {
	std::string num;
	while (std::isdigit(peek())) {
		num.push_back(next());
	}
	tokens.push_back({INTCON, num, curLineNum});
}

// alpha, digit or _
void Lexer::tokenizeWord() {
	std::string word;
	while (std::isalnum(peek()) || peek()=='_') {
		word.push_back(next());
	}
	if (keywordMap.find(word)!=keywordMap.end()) {
		tokens.push_back({keywordMap[word], word, curLineNum});
	} else {
		tokens.push_back({IDENFR, word, curLineNum});
	}
}

void Lexer::tokenizeOperatorOrPunctuation() {
	std::string op(1, next());
	static std::map<std::string, TokenType> opMap = {
		{"!", NOT},
		{"%", MOD},
		{"+", PLUS},
		{"-", MINU},
		{"*", MULT},
		{"/", DIV},
		{"=", ASSIGN},
		{"<", LSS},
		{">", GRE},
		{"(", LPARENT},
		{")", RPARENT},
		{"[", LBRACK},
		{"]", RBRACK},
		{"{", LBRACE},
		{"}", RBRACE},
		{";", SEMICN},
		{",", COMMA},
		{"<=", LEQ},
		{">=", GEQ},
		{"==", EQL},
		{"!=", NEQ},
		{"&&", AND},
		{"||", OR}
	};

	static std::unordered_set<std::string> doubleCharsOp{"<=", ">=", "==", "!=", "&&", "||"};
	if (peek() && doubleCharsOp.count(op + peek())) {
		op += next();
	}
	if (opMap.find(op)!=opMap.end()) {
		tokens.push_back({opMap[op], op, curLineNum});
	}
}

std::vector<Token> Lexer::analyze() {
	while (pos < code.size()) {
		tokenize();
	}
	return tokens;
}

