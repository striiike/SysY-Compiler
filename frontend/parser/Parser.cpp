//
// Created by hiccup on 2023/9/30.
//
#include "Parser.h"
#include <vector>

using namespace std;

DeclPtr Parser::parseDecl() {
	auto tokenType = tokenStream.peek()->type;
	bool isConst = tokenType==CONSTTK;
	if (isConst)
		tokenStream.next();
	TokenNode bType(*tokenStream.next());

	std::vector<DefPtr> defPtrs;
	defPtrs.push_back(parseDef(isConst));
	while (!tokenStream.reachEnd()) {
		if (tokenStream.peek()->type!=COMMA) {
			break;
		}
		auto comma = tokenStream.next();
		defPtrs.push_back(parseDef(isConst));
	}

	tokenStream.check(SEMICN, Exception::SEMICN_LACKED);
	return new Decl(isConst, bType, defPtrs);
}

DefPtr Parser::parseDef(bool isConst) {
	TokenNode ident(*tokenStream.next());

	std::vector<ExpPtr> constExpPtrs;
	while (!tokenStream.reachEnd()) {
		if (tokenStream.peek()->type!=LBRACK)
			break;
		auto left = tokenStream.next();
		constExpPtrs.push_back(parseConstExp());
		tokenStream.check(RBRACK, Exception::RBRACK_LACKED);
	}
	if (tokenStream.peek()->type==ASSIGN) {
		auto assign = tokenStream.next();
		return new Def(isConst, ident, constExpPtrs, parseInitVal(isConst));
	}
	return new Def(isConst, ident, constExpPtrs, nullptr);
}

InitValPtr Parser::parseInitVal(bool isConst) {
	if (tokenStream.peek()->type!=LBRACE)
		return parseExpInitVal(isConst);
	return parseArrayInitVal(isConst);
}

ExpInitValPtr Parser::parseExpInitVal(bool isConst) {
	return new ExpInitVal(isConst, isConst ? parseConstExp() : parseExp());
}

ArrayInitValPtr Parser::parseArrayInitVal(bool isConst) {
	// '{'
	auto left = tokenStream.next();
	std::vector<InitValPtr> initValPtrs;
	if (tokenStream.peek()->type==RBRACE) {
		// '}'
		auto right = tokenStream.next();
		return new ArrayInitVal(isConst, initValPtrs);
	}
	initValPtrs.push_back(parseInitVal(isConst));
	while (!tokenStream.reachEnd()) {
		if (tokenStream.peek()->type!=COMMA)
			break;
		tokenStream.next();
		initValPtrs.push_back(parseInitVal(isConst));
	}

	// '}'
	auto right = tokenStream.next();
	return new ArrayInitVal(isConst, initValPtrs);
}

ExpPtr Parser::parseExp() {
	return new Exp(std::move(parseAddExp()), false);
}

ExpPtr Parser::parseConstExp() {
	return new Exp(std::move(parseAddExp()), true);
}

CondPtr Parser::parseCond() {
	return new Cond(std::move(parseLOrExp()));
}

LValPtr Parser::parseLVal() {
	vector<ExpPtr> array;
	TokenNode ident(*tokenStream.next());
	while (!tokenStream.reachEnd()) {
		auto token = tokenStream.peek();
		if (!token || (token->type!=LBRACK))
			break;

		Token leftBracket = *tokenStream.next();
		ExpPtr expPtr = parseExp();

		tokenStream.check(RBRACK, Exception::RBRACK_LACKED);
		array.push_back(expPtr);
	}
	return new LVal(ident, array);
}

PrimaryExpPtr Parser::parsePrimaryExp() {
	auto token = tokenStream.peek();

	switch (token->type) {
	case LPARENT: {
		auto left = tokenStream.next();
		ExpPtr expPtr = parseExp();
		tokenStream.check(RPARENT, Exception::RPARENT_LACKED);
		return new PrimaryExp(expPtr, nullptr, nullptr);
	}
	case IDENFR:return new PrimaryExp(nullptr, parseLVal(), nullptr);
	case INTCON:return new PrimaryExp(nullptr, nullptr, parseNumber());
	default:return nullptr;
	}
}

NumberPtr Parser::parseNumber() {
	string str = tokenStream.next()->value;
	return new Number(str);
}

FunctionCallPtr Parser::parseFunctionCall() {
	TokenNode ident(*tokenStream.next());
	auto left = tokenStream.next();

	auto token = tokenStream.peek()->type;
	FuncRParamsPtr funcRParamsPtr = nullptr;
	if (token==IDENFR || token==LPARENT || token==INTCON
		|| token==PLUS || token==MINU || token==NOT) {
		funcRParamsPtr = parseFuncRParams();
	}
	tokenStream.check(RPARENT, Exception::RPARENT_LACKED);
	return new FunctionCall(ident, funcRParamsPtr);
}

FuncRParamsPtr Parser::parseFuncRParams() {
	vector<ExpPtr> expPtrs;
	expPtrs.push_back(parseExp());
	while (!tokenStream.reachEnd()) {
		if (tokenStream.peek()->type!=COMMA) {
			break;
		}
		tokenStream.next();
		expPtrs.push_back(parseExp());
	}
	return new FuncRParams(expPtrs);
}

UnaryExpPtr Parser::parseUnaryExp() {
	std::vector<TokenType> unaryOps;
	while (!tokenStream.reachEnd()) {
		if (tokenStream.peek()->type!=NOT && tokenStream.peek()->type!=PLUS && tokenStream.peek()->type!=MINU) {
			break;
		}
		unaryOps.push_back(tokenStream.next()->type);
		parseLog("<UnaryOp>");
	}

	auto token1 = tokenStream.peek()->type;
	auto token2 = tokenStream.peek(1)->type;
	if (token1==IDENFR && token2==LPARENT) {
		auto ret = new UnaryExp(unaryOps, nullptr, parseFunctionCall());
		for (int i = 0; i < unaryOps.size(); i++) {
			parseLog("<UnaryExp>");
		}
		return ret;
	} else {
		auto ret = new UnaryExp(unaryOps, parsePrimaryExp(), nullptr);
		for (int i = 0; i < unaryOps.size(); i++) {
			parseLog("<UnaryExp>");
		}
		return ret;
	}

}

MulExpPtr Parser::parseMulExp() {
	auto leftOperand = parseUnaryExp();
	vector<TokenType> operators;
	vector<UnaryExpPtr> operands;

	while (!tokenStream.reachEnd()) {
		auto token = tokenStream.peek();
		if (!token || (token->type!=MULT && token->type!=DIV && token->type!=MOD))
			break;
		parseLog("<MulExp>");
		operators.push_back(tokenStream.next()->type);
		operands.push_back(parseUnaryExp());
	}

	return new MulExp(std::move(leftOperand), std::move(operators), std::move(operands));
}

AddExpPtr Parser::parseAddExp() {
	auto leftOperand = parseMulExp();
	vector<TokenType> operators;
	vector<MulExpPtr> operands;

	while (!tokenStream.reachEnd()) {
		auto token = tokenStream.peek();
		if (!token || (token->type!=PLUS && token->type!=MINU))
			break;
		parseLog("<AddExp>");
		operators.push_back(tokenStream.next()->type); // Assuming next() will return the token
		operands.push_back(parseMulExp());
	}

	return new AddExp(std::move(leftOperand), std::move(operators), std::move(operands));
}

RelExpPtr Parser::parseRelExp() {
	auto leftOperand = parseAddExp();
	vector<TokenType> operators;
	vector<AddExpPtr> operands;

	while (!tokenStream.reachEnd()) {
		auto token = tokenStream.peek();
		if (!token || (token->type!=LSS && token->type!=GRE && token->type!=LEQ && token->type!=GEQ))
			break;
		parseLog("<RelExp>");
		operators.push_back(tokenStream.next()->type);
		operands.push_back(parseAddExp());
	}

	return new RelExp(std::move(leftOperand), std::move(operators), std::move(operands));
}

EqExpPtr Parser::parseEqExp() {
	auto leftOperand = parseRelExp();
	vector<TokenType> operators;
	vector<RelExpPtr> operands;

	while (!tokenStream.reachEnd()) {
		auto token = tokenStream.peek();
		if (!token || (token->type!=EQL && token->type!=NEQ))
			break;
		parseLog("<EqExp>");
		operators.push_back(tokenStream.next()->type);
		operands.push_back(parseRelExp());
	}

	return new EqExp(std::move(leftOperand), std::move(operators), std::move(operands));
}

LAndExpPtr Parser::parseLAndExp() {
	auto leftOperand = parseEqExp();
	vector<TokenType> operators;
	vector<EqExpPtr> operands;

	while (!tokenStream.reachEnd()) {
		auto token = tokenStream.peek();
		if (!token || token->type!=AND)
			break;
		parseLog("<LAndExp>");
		operators.push_back(tokenStream.next()->type);
		operands.push_back(parseEqExp());
	}

	return new LAndExp(std::move(leftOperand), std::move(operators), std::move(operands));
}

LOrExpPtr Parser::parseLOrExp() {
	auto leftOperand = parseLAndExp();
	vector<TokenType> operators;
	vector<LAndExpPtr> operands;

	while (!tokenStream.reachEnd()) {
		auto token = tokenStream.peek();
		if (!token || token->type!=OR)
			break;
		parseLog("<LOrExp>");

		operators.push_back(tokenStream.next()->type);
		operands.push_back(parseLAndExp());
	}

	return new LOrExp(std::move(leftOperand), std::move(operators), std::move(operands));
}

FuncDefPtr Parser::parseFuncDef() {
	TokenNode funcType(tokenStream.next().value());
	parseLog("<FuncType>");
	TokenNode ident(tokenStream.next().value());
	auto left = tokenStream.next();
	FuncFParamsPtr funcFParamsPtr = nullptr;
	auto token = tokenStream.peek()->type;
	if (token==INTTK) {
		funcFParamsPtr = parseFuncFParams();
	}
	tokenStream.check(RPARENT, Exception::RPARENT_LACKED);
	return new FuncDef(funcType, ident, funcFParamsPtr, parseBlock());
}

MainFuncDefPtr Parser::parseMainFuncDef() {
	TokenNode _int(tokenStream.next().value());
	TokenNode _main(tokenStream.next().value());
	auto left = tokenStream.next();
	tokenStream.check(RPARENT, Exception::RPARENT_LACKED);

	return new MainFuncDef(_int, _main, parseBlock());
}

FuncFParamsPtr Parser::parseFuncFParams() {
	std::vector<FuncFParamPtr> funcFParamPtrs;
	funcFParamPtrs.push_back(parseFuncFParam());
	while (!tokenStream.reachEnd()) {
		if (tokenStream.peek()->type!=COMMA)
			break;
		auto comma = tokenStream.next();
		funcFParamPtrs.push_back(parseFuncFParam());
	}
	return new FuncFParams(funcFParamPtrs);
}

FuncFParamPtr Parser::parseFuncFParam() {
	Token _bType = tokenStream.next().value();
	TokenNode bType(_bType);
	Token _ident = tokenStream.next().value();
	TokenNode ident(_ident);

	std::vector<ExpPtr> constExpPtrs{};

	if (tokenStream.peek()->type!=LBRACK)
		return new FuncFParam(bType, ident, false, constExpPtrs);
	auto left = tokenStream.next();
	tokenStream.check(RBRACK, Exception::RBRACK_LACKED);
	while (!tokenStream.reachEnd()) {
		if (tokenStream.peek()->type!=LBRACK)
			break;
		auto _left = tokenStream.next();
		constExpPtrs.push_back(parseConstExp());
		tokenStream.check(RBRACK, Exception::RBRACK_LACKED);
	}
	return new FuncFParam(bType, ident, true, constExpPtrs);
}

BlockPtr Parser::parseBlock() {
	auto left = tokenStream.next();
	std::vector<BlockItemPtr> blockItemPtrs;
	while (!tokenStream.reachEnd()) {
		if (tokenStream.peek()->type==RBRACE)
			break;
		blockItemPtrs.push_back(parseBlockItem());
	}
	TokenNode right(*tokenStream.next());
	return new Block(blockItemPtrs, right);
}

BlockItemPtr Parser::parseBlockItem() {
	TokenType token = tokenStream.peek()->type;
	if (token==CONSTTK || token==INTTK) {
		return new BlockItem(parseDecl(), nullptr);
	}
	return new BlockItem(nullptr, parseStmt());
}

StmtPtr Parser::parseStmt() {
	if (tokenStream.peek()->type==SEMICN) {
		auto semicn = tokenStream.next();
		parseLog("<Stmt>");
		return new Stmt();
	}
	if (tokenStream.peek()->type==IFTK ||
		tokenStream.peek()->type==FORTK ||
		tokenStream.peek()->type==LBRACE) {
		auto ret = parseComplexStmt();
		parseLog("<Stmt>");
		return ret;
	}

	auto simpleStmtPtr = parseSimpleStmt();
	tokenStream.check(SEMICN, Exception::SEMICN_LACKED);
	parseLog("<Stmt>");
	return simpleStmtPtr;
}

/// used to check whether Exp is LVal, \only for parseSimpleStmt
LValPtr getLValPtr(const ExpPtr &expPtr) {
	auto add = expPtr->addExpPtr;
	auto mul = add->getLeftOperand();
	auto unary = mul->getLeftOperand();
	auto ops = unary->getUnaryOps();
	if (!ops.empty())
		return nullptr;
	auto primary = unary->getPrimaryExpPtr();
	if (primary)
		return primary->getLValPtr();
	else
		return nullptr;
}

SimpleStmtPtr Parser::parseAssignOrGetintStmt(const LValPtr &lValPtr) {
	auto assign = tokenStream.next();
	if (tokenStream.peek()->type==GETINTTK) {
		TokenNode _getint(*tokenStream.next());
		auto left = tokenStream.next();
		tokenStream.check(RPARENT, Exception::RPARENT_LACKED);
		return new GetintStmt(lValPtr, _getint);
	}
	return new AssignStmt(lValPtr, parseExp());
}

SimpleStmtPtr Parser::parseSimpleStmt() {
	auto token = *tokenStream.peek();
	if (token.type==BREAKTK) {
		TokenNode _break(token);
		tokenStream.next();
		return new BreakStmt(_break);
	}
	if (token.type==CONTINUETK) {
		TokenNode _continue(token);
		tokenStream.next();
		return new ContinueStmt(_continue);
	}
	if (token.type==RETURNTK) {
		TokenNode _return(token);
		tokenStream.next();
		ExpPtr expPtr = nullptr;

		TokenType type = tokenStream.peek()->type;
		if (type==IDENFR || type==LPARENT || type==INTCON
			|| type==PLUS || type==MINU || type==NOT) {
			expPtr = parseExp();
		}
		return new ReturnStmt(_return, expPtr);
	}
	if (token.type==PRINTFTK) {
		TokenNode _printf(token);
		tokenStream.next();
		auto left = tokenStream.next();
		TokenNode formatStringToken(*tokenStream.next());
		std::vector<ExpPtr> expPtrs;
		while (!tokenStream.reachEnd()) {
			if (tokenStream.peek()->type!=COMMA)
				break;
			tokenStream.next();
			expPtrs.push_back(parseExp());
		}
		tokenStream.check(RPARENT, Exception::RPARENT_LACKED);
		return new PrintfStmt(_printf, formatStringToken, expPtrs);
	}


	/// \very_servere_problem_!!
	bool isAssignOrGetint = false;
	PARSER_DISPLAY = false;
	{
		/// \NO_NEED_TO_WORRY_SIDE_EFFECT_OF_COPY
		TokenStream tokenStreamCopy1(tokenStream);
		TokenStream tokenStreamCopy2(tokenStream);

		Parser ParserCopy1(tokenStreamCopy1);
		Parser ParserCopy2(tokenStreamCopy2);
		ExpPtr expPtr = ParserCopy1.parseExp();
		LValPtr lValPtr = getLValPtr(expPtr);
		if (lValPtr) {
			LValPtr lValPtr1 = ParserCopy2.parseLVal();
			isAssignOrGetint = (tokenStreamCopy2.peek()->type==ASSIGN);
		}
	}
	PARSER_DISPLAY = true;

	if (isAssignOrGetint) {
		return parseAssignOrGetintStmt(parseLVal());
	} else {
		return new ExpStmt(parseExp());
	}
}

_ForStmtPtr Parser::parse_ForStmt() {
	LValPtr lValPtr = parseLVal();
	auto assign = tokenStream.next();
	return new _ForStmt(lValPtr, parseExp());
}

ComplexStmtPtr Parser::parseComplexStmt() {
	if (tokenStream.peek()->type==IFTK) {
		auto _if = tokenStream.next();
		auto left = tokenStream.next();
		CondPtr condPtr = parseCond();
		tokenStream.check(RPARENT, Exception::RPARENT_LACKED);
		StmtPtr stmtPtr1 = parseStmt();
		if (tokenStream.peek()->type==ELSETK) {
			auto _else = tokenStream.next();
			return new IfStmt(condPtr, stmtPtr1, parseStmt());
		}
		return new IfStmt(condPtr, stmtPtr1, nullptr);
	}
	if (tokenStream.peek()->type==FORTK) {
		auto _for = tokenStream.next();
		auto left = tokenStream.next();
		_ForStmtPtr assignStmtPtr1 = nullptr;
		if (tokenStream.peek()->type!=SEMICN) {
			assignStmtPtr1 = parse_ForStmt();
		}
		tokenStream.check(SEMICN, Exception::SEMICN_LACKED);
		CondPtr condPtr = nullptr;
		if (tokenStream.peek()->type!=SEMICN) {
			condPtr = parseCond();
		}
		tokenStream.check(SEMICN, Exception::SEMICN_LACKED);
		_ForStmtPtr assignStmtPtr2 = nullptr;

		if (tokenStream.peek()->type!=RPARENT) {
			assignStmtPtr2 = parse_ForStmt();
		}
		tokenStream.check(RPARENT, Exception::RPARENT_LACKED);
		return new ForStmt(condPtr, parseStmt(), assignStmtPtr1, assignStmtPtr2);
	}
	if (tokenStream.peek()->type==LBRACE) {
		return parseBlock();
	}

	/// \error
	return nullptr;
}

CompUnit Parser::parseCompUnit() {
	MainFuncDefPtr _main;
	std::vector<DeclPtr> declPtrs;
	std::vector<FuncDefPtr> funcDefPtrs;
	while (!tokenStream.reachEnd()) {
		auto type = tokenStream.peek()->type;
		auto type1 = tokenStream.peek(1)->type;
		auto type2 = tokenStream.peek(2)->type;
		if (type==INTTK && type1==MAINTK && type2==LPARENT) {
			_main = parseMainFuncDef();
		} else if ((type==VOIDTK || type==INTTK) && type1==IDENFR && type2==LPARENT) {
			funcDefPtrs.push_back(parseFuncDef());
		} else {
			declPtrs.push_back(parseDecl());
		}
	}
	return CompUnit(declPtrs, funcDefPtrs, _main);
}
