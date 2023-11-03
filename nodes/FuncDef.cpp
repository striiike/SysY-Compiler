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

Value *FuncDef::llvmIr() {
	irBuilder.ctx.isGlobal = false;
	irBuilder.ctx.afterFuncDef = true;
	irBuilder.ctx.voidFunc = funcType.getType() == VOIDTK;
	symbol.startScope();

	Type *ty = funcType.getType() == INTTK ? IntegerType::INT32 : IntegerType::VOID;

	auto name = irBuilder.genFuncName(ident.getValue());
	auto *func = new Function(ty, name);
	irBuilder.addFunction(func);
	irBuilder.setCurFunc(func);

	vector<int> dims{};
	if (funcFParamsPtr) {
		dims = funcFParamsPtr->getDims();
	}
	symbol.insertFunc(ident.getValue(), funcType.getType() == VOIDTK, dims, func);

	auto bb = new BasicBlock(irBuilder.genBbName());
	irBuilder.addBasicBlock(bb);
	irBuilder.setCurBb(bb);

	if (funcFParamsPtr)
		funcFParamsPtr->llvmIr();
	blockPtr->llvmIr();

	irBuilder.fillInReturn();

	irBuilder.ctx.afterFuncDef = false;
	return nullptr;

}