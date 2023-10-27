//
// Created by hiccup on 2023/10/7.
//

#include "UnaryExp.h"
#include "PrimaryExp.h"
#include "Exp.h"
#include "FunctionCall.h"

UnaryExp::UnaryExp(std::vector<TokenType> unaryOps, PrimaryExpPtr primaryExpPtr, FunctionCallPtr functionCallPtr)
	: unaryOps(std::move(unaryOps)), primaryExpPtr(std::move(primaryExpPtr)),
	  functionCallPtr(std::move(functionCallPtr)) {
	name = "<UnaryExp>";
	print();
}

std::vector<TokenType> &UnaryExp::getUnaryOps() {
	return unaryOps;
}

PrimaryExpPtr UnaryExp::getPrimaryExpPtr() {
	return primaryExpPtr;
}

FunctionCallPtr UnaryExp::getFunctionCallPtr() {
	return functionCallPtr;
}

void UnaryExp::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	auto _ret = make_shared<ErrorRet>();
	if (primaryExpPtr) {
		primaryExpPtr->checkError(ctx, _ret);
	} else if (functionCallPtr) {
		functionCallPtr->checkError(ctx, _ret);
	}
	ret->dim = _ret->dim;
}

int UnaryExp::evaluate() {
	/// when evaluating in global variable analysis,
	/// primaryExp is the \only possible one
	int res = primaryExpPtr->evaluate();
	size_t negationCount = std::count(unaryOps.begin(), unaryOps.end(), MINU);
	return (negationCount % 2) ? -res : res;
}

Value *UnaryExp::llvmIr() {
	ASTNode::llvmIr();
}
