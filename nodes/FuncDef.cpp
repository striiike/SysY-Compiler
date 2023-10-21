#include "FuncDef.h"

FuncDef::FuncDef(TokenNode funcType, TokenNode ident, FuncFParamsPtr funcFParamsPtr, BlockPtr blockPtr)
	: funcType(std::move(funcType)), ident(std::move(ident)),
	  funcFParamsPtr(std::move(funcFParamsPtr)),
	  blockPtr(std::move(blockPtr)) {
	name = "<FuncDef>";
	print();
}

void FuncDef::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	vector<int> dims{};
	if (funcFParamsPtr) {
		dims = funcFParamsPtr->getDims();
	}
	// no detail for check error
	if (!symbol.insertFunc(ident.getValue(), funcType.getType() == VOIDTK, dims)) {
		errorList.emplace_back(Exception::REDEFINED_IDENT, ident.getLineNum());
	}

	symbol.startScope();
	ret = make_shared<ErrorRet>();
	if (funcFParamsPtr)
		funcFParamsPtr->checkError(ctx, ret);
	ctx->afterFuncDef = true;
	ctx->voidFunc = (funcType.getType() == VOIDTK);
	blockPtr->checkError(ctx, ret);

	if (!ctx->voidFunc && !ret->hasRet) {
		errorList.emplace_back(Exception::INT_RETURN_LACKED, ret->rbraceLineNum);
	}
}

void FuncDef::llvmIr() {
//	ctx->isGlobal = false;

}