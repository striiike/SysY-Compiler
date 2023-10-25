//
// Created by hiccup on 2023/10/9.
//

#include "LVal.h"
#include "Exp.h"
#include "../parser/Symbol.hpp"

LVal::LVal(TokenNode ident, std::vector<ExpPtr> array) : ident(std::move(ident)), array(std::move(array)) {
	name = "<LVal>";
	print();
}

void LVal::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	auto varPtr = symbol.getVar(ident.getValue());
	vector<int> lens{};
	if (varPtr)
		lens = varPtr->lens;
	if (!varPtr) {
		errorList.emplace_back(Exception::UNDEFINED_IDENT, ident.getLineNum());
		ret->undefined = true;
	} else if (varPtr->isConst && ctx->isLeftValue)
		errorList.emplace_back(Exception::CONST_ASSIGNED, ident.getLineNum());
	for (const auto &i : array) {
		i->checkError(ctx, ret);
	}

	ret->dim = (int)lens.size() - (int)array.size();
}

int LVal::evaluate() {
	vector<int> index{};
	index.reserve(array.size());
	for (const auto &i : array) {
		index.push_back(i->evaluate());
	}
	auto varPtr = symbol.getVar(ident.getValue());
	int res = varPtr->getValue(index);
	return res;
}