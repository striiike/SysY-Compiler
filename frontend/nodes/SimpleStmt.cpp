#include "../nodes-include/SimpleStmt.h"
#include "../nodes-include/Exp.h"
#include "../parser/Symbol.hpp"
#include "../nodes-include/LVal.h"
#include "../../llvm-ir/IrBuilder.h"

AssignStmt::AssignStmt(LValPtr lValPtr, ExpPtr expPtr)
	: lValPtr(std::move(lValPtr)), expPtr(std::move(expPtr)), SimpleStmt() {}

void AssignStmt::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	ctx->isLeftValue = true;
	lValPtr->checkError(ctx, ret);
	ctx->isLeftValue = false;
	expPtr->checkError(ctx, ret);
}

Value *AssignStmt::llvmIr() {
	auto *rvalue = expPtr->llvmIr();
	auto *lval = lValPtr->llvmIrAddr();
	irBuilder.buildStore(rvalue, lval);
	/// no need for sysY
	return nullptr;
}

ExpStmt::ExpStmt(ExpPtr expPtr)
	: expPtr(std::move(expPtr)), SimpleStmt() {}

void ExpStmt::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	expPtr->checkError(ctx, ret);
}
Value *ExpStmt::llvmIr() {
	return expPtr->llvmIr();
}

BreakStmt::BreakStmt(TokenNode _break)
	: _break(std::move(_break)), SimpleStmt() {}

void BreakStmt::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	if (!ctx->loopNum)
		errorList.emplace_back(Exception::BREAK_CONTINUE_OUT_LOOP, _break.getLineNum());
}
Value *BreakStmt::llvmIr() {
	irBuilder.buildBrInst(irBuilder.endStack.top());
	return nullptr;
}

ContinueStmt::ContinueStmt(TokenNode _continue)
	: _continue(std::move(_continue)), SimpleStmt() {}

void ContinueStmt::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	if (!ctx->loopNum)
		errorList.emplace_back(Exception::BREAK_CONTINUE_OUT_LOOP, _continue.getLineNum());
}
Value *ContinueStmt::llvmIr() {
	irBuilder.buildBrInst(irBuilder.incStack.top());
	return nullptr;
}

ReturnStmt::ReturnStmt(TokenNode _return, ExpPtr expPtr)
	: _return(std::move(_return)), expPtr(std::move(expPtr)), SimpleStmt() {}

void ReturnStmt::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	if (expPtr) {
		expPtr->checkError(ctx, ret);
		if (ctx->voidFunc) {
			errorList.emplace_back(Exception::VOID_RETURN_INT, _return.getLineNum());
		}
	}
	ret->hasRet = true;
	ret->retLineNum = _return.getLineNum();
}

Value *ReturnStmt::llvmIr() {
	if (expPtr) {
		auto *ret = expPtr->llvmIr();
		irBuilder.buildReturn(ret);
	} else
		irBuilder.buildReturn(new Value(IntegerType::VOID, ""));
	return nullptr;
}

GetintStmt::GetintStmt(LValPtr lValPtr, TokenNode _getint)
	: lValPtr(std::move(lValPtr)), _getint(std::move(_getint)), SimpleStmt() {}

void GetintStmt::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	ctx->isLeftValue = true;
	lValPtr->checkError(ctx, ret);
	ctx->isLeftValue = false;
}
Value *GetintStmt::llvmIr() {
	auto *rvalue = irBuilder.buildCall(Function::getint, vector<Value *>{});
	auto *lval = lValPtr->llvmIrAddr();
	irBuilder.buildStore(rvalue, lval);
	/// no need for sysY
	return nullptr;
}

PrintfStmt::PrintfStmt(TokenNode _printf, TokenNode FormatString, std::vector<ExpPtr> expPtrs)
	: _printf(std::move(_printf)), formatString(std::move(FormatString)), expPtrs(std::move(expPtrs)),
	  SimpleStmt() {}

void PrintfStmt::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	auto str = formatString.getValue();

	// skip " "
	bool formatUnmatched = false;
	int cnt = 0;
	for (int i = 1; i < str.size() - 1; i++) {
		if (str[i]=='\\' && str[i + 1]!='n')
			formatUnmatched = true;
		if (str[i]!=32 && str[i]!=33 && (str[i] < 40 || str[i] > 126) && str[i]!='%')
			formatUnmatched = true;
		if (str[i]=='%') {
			if (str[i + 1]=='d')
				cnt++;
			else
				formatUnmatched = true;
		}
	}
	if (formatUnmatched)
		errorList.emplace_back(Exception::INVALID_CHARACTER, formatString.getLineNum());
	if (cnt!=expPtrs.size())
		errorList.emplace_back(Exception::FORMAT_CHAR_UNMATCHED, _printf.getLineNum());
	for (const auto &i : expPtrs)
		i->checkError(ctx, ret);
}

vector<string> splitStrings(const string &str) {
	std::stringstream ss(str.substr(1, str.size() - 2));
	std::vector<std::string> tokens;

	std::string token;
	while (getline(ss, token, '%')) {
		tokens.push_back({token + "\0"});
		if (ss.peek()=='d') {
			tokens.emplace_back("%d");
			ss.ignore();
		}
	}
	tokens.erase(remove(tokens.begin(), tokens.end(), ""), tokens.end());

	return tokens;
}

Value *PrintfStmt::llvmIr() {
	vector<Value *> args{};
	args.reserve(expPtrs.size());
	for (auto &i : expPtrs) {
		args.push_back(i->llvmIr());
	}

	vector<string> strings = splitStrings(formatString.getValue());
	int index = 0;
	for (auto &i : strings) {
		if (i=="%d") {
			irBuilder.buildCall(Function::putint, vector<Value *>{args[index++]});
		} else {
			auto *str = new ConstantString(i);
			auto *g = irBuilder.buildGlobalVar("", str, true);
			auto *gep = irBuilder.buildGEP(g, new ConstantInt(0));
			irBuilder.buildCall(Function::putstr, vector<Value *>{gep});
		}
	}
	return nullptr;
}



































