#include "Def.h"

Def::Def(bool isConst, TokenNode ident, std::vector<ExpPtr> expPtrs, InitValPtr initValPtr)
	: isConst(isConst), ident(std::move(ident)), expPtrs(std::move(expPtrs)),
	  initValPtr(std::move(initValPtr)) {
	name = isConst ? "<ConstDef>" : "<VarDef>";
	print();
}

void Def::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	for (auto &i : expPtrs) {
		i->checkError(ctx, ret);
	}

	if (initValPtr)
		initValPtr->checkError(ctx, ret);
	// no detail
	auto lens = vector<int>(expPtrs.size());
	if (!symbol.insertVar(isConst, ident.getValue(), lens)) {
		errorList.emplace_back(Exception::REDEFINED_IDENT, ident.getLineNum());
	}
}
void Def::llvmIr() {

}
