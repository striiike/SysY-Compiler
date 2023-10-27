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

void Def::buildGlobalDef(const vector<int> &lens, int length) {
	vector<int> initValue{};
	if (initValPtr)
		initValue = initValPtr->evaluate();

	Constant *constant;
	// single variable
	if (expPtrs.empty()) {
		constant = new ConstantInt(initValPtr ? initValue[0] : 0, 32);
	} else {
		constant = new ConstantArray(initValue, length);
	}

	irBuilder.buildGlobalVar(ident.getValue(), constant, isConst);
	symbol.insertVar(isConst, ident.getValue(), lens, initValue);
}

void Def::buildLocalDef(const vector<int> &lens, int length) {
	vector<int> initValue{};

	if (isConst) {

		Constant *constant;
		if (expPtrs.empty()) {
			/// \SEEMS_NO \TODO
		} else {
			Value *alloca = irBuilder.buildAlloc(ident.getValue(), new ArrayType(length, IntegerType::INT32));
			if (initValPtr) {
				initValue = initValPtr->evaluate();
				for (int i = 0; i < length; ++i) {
					Value *gep = irBuilder.buildGEP(
						alloca,
						new ConstantInt(0),
						new ConstantInt(i)
					);
					irBuilder.buildStore(new ConstantInt(initValue[i]), gep);
				}
			}
		}

	} else {

		if (expPtrs.empty()) {
			Value *alloca = irBuilder.buildAlloc(ident.getValue(), IntegerType::INT32);
			if (initValPtr) {
				Value *value = initValPtr->llvmIr();
				irBuilder.buildStore(value, alloca);
			}
		} else {
			/// \TODO
		}

	}
	symbol.insertVar(isConst, ident.getValue(), lens, initValue);
}

Value *Def::llvmIr() {

	/// grammar @ensures that there is no VLA
	vector<int> dims{};
	int length = 1;
	dims.reserve(expPtrs.size());
	for (auto &i : expPtrs) {
		int val = i->evaluate();
		dims.push_back(val);
		length *= val;
	}

	/// ----------------- building llvm ir starts here ----------------- ///
	if (irBuilder.ctx.isGlobal) {
		buildGlobalDef(dims, length);
	} else {
		buildLocalDef(dims, length);
	}

	return nullptr;
}
