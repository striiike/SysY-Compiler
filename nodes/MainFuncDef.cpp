#include "MainFuncDef.h"
#include "ComplexStmt.h"

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
