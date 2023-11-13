#include "../nodes-include/FuncFParam.h"
#include "../nodes-include/Exp.h"
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

Value *FuncFParam::llvmIr() {
	vector<int> lens = {};
	if (isArray)
		lens.push_back(0);
	for (auto &i : expPtrs) {
		lens.push_back(i->evaluate());
	}

	Type *ty;
	if (isArray) {
		ty = new PointerType(IntegerType::INT32);
	} else {
		ty = IntegerType::INT32;
	}

	auto *arg = irBuilder.buildArgument(ident.getValue(), ty);
	if (!isArray) {
		auto *alloc = irBuilder.buildAlloc(ident.getValue(), IntegerType::INT32);
		irBuilder.buildStore(arg, alloc);
		symbol.insertVar(false, ident.getValue(), lens, vector<int>{}, alloc);
	} else {
		symbol.insertVar(false, ident.getValue(), lens, vector<int>{}, arg);
	}

	return nullptr;
}