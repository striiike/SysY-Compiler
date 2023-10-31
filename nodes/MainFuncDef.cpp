#include "MainFuncDef.h"
#include "ComplexStmt.h"
#include "Block.h"

MainFuncDef::MainFuncDef(TokenNode bType, TokenNode _main, BlockPtr blockPtr) :
	bType(std::move(bType)), _main(std::move(_main)), blockPtr(std::move(blockPtr)) {
	name = "<MainFuncDef>";
	print();
}

void MainFuncDef::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	ctx->afterFuncDef = true;
	ctx->voidFunc = false;
	symbol.startScope();

	ret = make_shared<ErrorRet>();
	blockPtr->checkError(ctx, ret);
	ctx->afterFuncDef = false;

	if (!ret->hasRet) {
		errorList.emplace_back(Exception::INT_RETURN_LACKED, ret->rbraceLineNum);
	}
}

Value *MainFuncDef::llvmIr() {
	irBuilder.ctx.isGlobal = false;
	irBuilder.ctx.afterFuncDef = true;
	irBuilder.ctx.voidFunc = false;
	symbol.startScope();

	Type *ty = bType.getType() == INTTK ? IntegerType::INT32 : IntegerType::VOID;
	auto name = irBuilder.genFuncName("main");
	auto *func = new Function(ty, name);
	irBuilder.addFunction(func);
	irBuilder.setCurFunc(func);

	symbol.insertFunc("main", bType.getType() == VOIDTK);

	auto bb = new BasicBlock(irBuilder.genBbName());
	irBuilder.addBasicBlock(bb);
	irBuilder.setCurBb(bb);

	blockPtr->llvmIr();

	irBuilder.ctx.afterFuncDef = false;
	return nullptr;
}