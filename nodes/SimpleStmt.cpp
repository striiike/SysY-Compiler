#include "SimpleStmt.h"
#include "Exp.h"
#include "../parser/Symbol.hpp"
#include "LVal.h"

AssignStmt::AssignStmt(LValPtr lValPtr, ExpPtr expPtr)
	: lValPtr(std::move(lValPtr)), expPtr(std::move(expPtr)), SimpleStmt() {}

void AssignStmt::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	ctx->isLeftValue = true;
	lValPtr->checkError(ctx, ret);
	ctx->isLeftValue = false;
	expPtr->checkError(ctx, ret);
}

Value *AssignStmt::llvmIr() {
	SimpleStmt::llvmIr();
}

ExpStmt::ExpStmt(ExpPtr expPtr)
	: expPtr(std::move(expPtr)), SimpleStmt() {}

void ExpStmt::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	expPtr->checkError(ctx, ret);
}
Value *ExpStmt::llvmIr() {
	SimpleStmt::llvmIr();
}

BreakStmt::BreakStmt(TokenNode _break)
	: _break(std::move(_break)), SimpleStmt() {}

void BreakStmt::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	if (!ctx->loopNum)
		errorList.emplace_back(Exception::BREAK_CONTINUE_OUT_LOOP, _break.getLineNum());
}
Value *BreakStmt::llvmIr() {
	SimpleStmt::llvmIr();
}

ContinueStmt::ContinueStmt(TokenNode _continue)
	: _continue(std::move(_continue)), SimpleStmt() {}

void ContinueStmt::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	if (!ctx->loopNum)
		errorList.emplace_back(Exception::BREAK_CONTINUE_OUT_LOOP, _continue.getLineNum());
}
Value *ContinueStmt::llvmIr() {
	SimpleStmt::llvmIr();
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

}

GetintStmt::GetintStmt(LValPtr lValPtr, TokenNode _getint)
	: lValPtr(std::move(lValPtr)), _getint(std::move(_getint)), SimpleStmt() {}

void GetintStmt::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	ctx->isLeftValue = true;
	lValPtr->checkError(ctx, ret);
	ctx->isLeftValue = false;
}
Value *GetintStmt::llvmIr() {
	SimpleStmt::llvmIr();
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
		if (str[i] == '\\' && str[i + 1] != 'n')
			formatUnmatched = true;
		if (str[i] != 32 && str[i] != 33 && (str[i] < 40 || str[i] > 126) && str[i] != '%')
			formatUnmatched = true;
		if (str[i] == '%') {
			if (str[i + 1] == 'd')
				cnt++;
			else
				formatUnmatched = true;
		}
	}
	if (formatUnmatched)
		errorList.emplace_back(Exception::INVALID_CHARACTER, formatString.getLineNum());
	if (cnt != expPtrs.size())
		errorList.emplace_back(Exception::FORMAT_CHAR_UNMATCHED, _printf.getLineNum());
	for (const auto &i : expPtrs)
		i->checkError(ctx, ret);
}

Value *PrintfStmt::llvmIr() {
	SimpleStmt::llvmIr();
}



































