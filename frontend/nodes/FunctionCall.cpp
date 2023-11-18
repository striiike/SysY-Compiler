//
// Created by hiccup on 2023/10/9.
//

#include "../nodes-include/FunctionCall.h"
#include "../nodes-include/Exp.h"
#include "../parser/Symbol.hpp"
#include "../../llvm-ir/IrBuilder.h"

FuncRParams::FuncRParams(std::vector<ExpPtr> expPtrs) : expPtrs(std::move(expPtrs)) {
	name = "<FuncRParams>";
	print();
}

void FuncRParams::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	vector<int> dims{};
	for (const auto &i : expPtrs) {
		auto _ret = make_shared<ErrorRet>();
		i->checkError(ctx, _ret);
		dims.push_back(_ret->dim);
	}
	ret->dims = dims;
}

vector<Value *> FuncRParams::llvmIrList() {
	vector<Value *> args{};
	args.reserve(expPtrs.size());
	for (const auto &i : expPtrs) {
		args.push_back(i->llvmIr());
	}
	return args;
}

FunctionCall::FunctionCall(TokenNode ident, FuncRParamsPtr funcRParamsPtr)
	: ident(std::move(ident)), funcRParamsPtr(std::move(funcRParamsPtr)) {
	name = "<FunctionCall>";
}

void FunctionCall::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	auto funcPtr = symbol.getFunc(ident.getValue());

	vector<int> dims{};
	if (funcPtr) {
		dims = funcPtr->dims;
		ret->dim = (funcPtr->isVoid) ? -1 : 0;
	} else {
		errorList.emplace_back(Exception::UNDEFINED_IDENT, ident.getLineNum());
		ret->dim = 0;
	}

	if (funcRParamsPtr)
		funcRParamsPtr->checkError(ctx, ret);

	if (dims.size()!=ret->dims.size())
		errorList.emplace_back(Exception::PARAMS_NUM_UNMATCHED, ident.getLineNum());
	else if (!ret->undefined) {
		for (int i = 0; i < dims.size(); ++i) {
			if (dims[i]!=ret->dims[i]) {
				errorList.emplace_back(Exception::PARAM_TYPE_UNMATCHED, ident.getLineNum());
				break;
			}
		}
	}
}

Value *FunctionCall::llvmIr() {
	vector<Value *> args{};
	if (funcRParamsPtr)
		args = funcRParamsPtr->llvmIrList();
	auto funcPtr = symbol.getFunc(ident.getValue());
	auto *call = irBuilder.buildCall((Function *)funcPtr->getLlvmValue(), args);
	return call;
}
