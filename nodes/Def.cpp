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

	Type *ty;
	vector<int> lens{};
	int num = 1;
	lens.reserve(expPtrs.size());
	for (auto &i : expPtrs) {
		int val = i->evaluate();
		lens.push_back(val);
		num *= val;
	}
	if (expPtrs.empty()) {
		ty = new IntegerType(32);
	} else {
		ty = new ArrayType(num, new IntegerType(32));
	}

	vector<int> initValue = initValPtr ? std::move(initValPtr->evaluate()) : vector<int>{};
	auto *init = new Initializer(ty, initValue);

	if (irBuilder.ctx.isGlobal) {
		/// it \seems no need to llvmIr indexes
		string name = irBuilder.genGlobalVarName(ident.getValue());
		irBuilder.addGlobalVariable(new GlobalVariable(nullptr, name, init));
	}

	symbol.insertVar(isConst, ident.getValue(), lens, initValue);
//	cout << "!!" + name << " " << num << " " << expPtrs.empty() << endl;
}
