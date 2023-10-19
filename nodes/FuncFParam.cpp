#include "FuncFParam.h"
#include "Exp.h"
#include "../parser/Symbol.hpp"

FuncFParam::FuncFParam(TokenNode bType, TokenNode ident, bool isArray, std::vector<ExpPtr> expPtrs)
	: bType(std::move(bType)),
	  ident(std::move(ident)),
	  isArray(isArray),
	  expPtrs(std::move(expPtrs)) {
	name = "<FuncFParam>";
	print();
}

int FuncFParam::getDim() {
	return (isArray) ? (int)expPtrs.size() + 1 : 0;
}

void FuncFParam::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	for (const auto &i : expPtrs) {
		i->checkError(ctx, ret);
	}
	vector<int> lens((isArray) ? expPtrs.size() + 1 : 0);

	if (!symbol.insertVar(false, ident.getValue(), lens)) {
		errorList.emplace_back(Exception::REDEFINED_IDENT, ident.getLineNum());
	}
}
