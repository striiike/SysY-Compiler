#include "../nodes-include/CompUnit.h"
#include "../nodes-include/Decl.h"
#include "../nodes-include/FuncDef.h"
#include "../nodes-include/MainFuncDef.h"
#include "../parser/Symbol.hpp"
#include "../../llvm-ir/IrBuilder.h"


CompUnit::CompUnit(std::vector<DeclPtr> declPtrs, std::vector<FuncDefPtr> funcDefPtrs, MainFuncDefPtr mainFuncDefPtr) :
	declPtrs(std::move(declPtrs)),
	funcDefPtrs(std::move(funcDefPtrs)),
	mainFuncDefPtr(mainFuncDefPtr) {
	name = "<CompUnit>";
	print();
}

void CompUnit::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	symbol.startScope();
	for (const auto &i : declPtrs)
		i->checkError(ctx, ret);
	for (const auto &i : funcDefPtrs)
		i->checkError(ctx, ret);
	mainFuncDefPtr->checkError(ctx, ret);
	symbol.endScope();
}

Value *CompUnit::llvmIr() {
	irBuilder.setModule(new Module());
	symbol.startScope();
	irBuilder.ctx.isGlobal = true;
	for (const auto &i : declPtrs)
		i->llvmIr();
	for (const auto &i : funcDefPtrs)
		i->llvmIr();
	mainFuncDefPtr->llvmIr();
	symbol.endScope();
	return nullptr;
}
