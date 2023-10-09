#include "MainFuncDef.h"
#include "ComplexStmt.h"

MainFuncDef::MainFuncDef(TokenNode bType, TokenNode _main, BlockPtr blockPtr) :
        bType(std::move(bType)), _main(std::move(_main)), blockPtr(std::move(blockPtr)) {
    name = "<MainFuncDef>";
    print();
}

void MainFuncDef::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
    ctx->inFunc = true;
    ctx->voidFunc = false;
    blockPtr->checkError(ctx, ret);
    ctx->inFunc = false;
}
