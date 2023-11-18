//
// Created by hiccup on 2023/10/7.
//

#include "../nodes-include/UnaryExp.h"
#include "../nodes-include/PrimaryExp.h"
#include "../nodes-include/Exp.h"
#include "../nodes-include/FunctionCall.h"

#include "../nodes-include/TokenNode.h"
#include "../../llvm-ir/IrBuilder.h"

UnaryExp::UnaryExp(std::vector<TokenType> unaryOps, PrimaryExpPtr primaryExpPtr, FunctionCallPtr functionCallPtr)
	: unaryOps(std::move(unaryOps)), primaryExpPtr(primaryExpPtr),
	  functionCallPtr(functionCallPtr) {
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
	return (negationCount%2) ? -res : res;
}

Value *UnaryExp::llvmIr() {
	Value *ret;
	if (primaryExpPtr)
		ret = primaryExpPtr->llvmIr();
	else if (functionCallPtr)
		ret = functionCallPtr->llvmIr();

	size_t negationCount = std::count(unaryOps.begin(), unaryOps.end(), MINU);
	if (negationCount%2) {
		ret = irBuilder.buildAlu(AluType::SUB, new ConstantInt(0), ret);
	}
	size_t notCount = std::count(unaryOps.begin(), unaryOps.end(), NOT);
	if (notCount%2) {
		ret = irBuilder.buildIcmpInst(IcmpType::EQ, ret, new ConstantInt(0));
		ret = irBuilder.buildZext(ret, IntegerType::INT32);
	}
	return ret;
}
