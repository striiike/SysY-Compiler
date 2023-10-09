#include "FuncDef.h"

FuncDef::FuncDef(TokenNode funcType, TokenNode ident, FuncFParamsPtr funcFParamsPtr, BlockPtr blockPtr)
        : funcType(std::move(funcType)), ident(std::move(ident)),
          funcFParamsPtr(std::move(funcFParamsPtr)),
          blockPtr(std::move(blockPtr)) {
    name = "<FuncDef>";
    print();
}

void FuncDef::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
    if (funcFParamsPtr) funcFParamsPtr->checkError(ctx, ret);
    ctx->inFunc = true;
    ctx->voidFunc = (funcType.getType() == VOIDTK);
    blockPtr->checkError(ctx, ret);
    ctx->inFunc = false;

    // no detail for check error
    if (!symbol.insertFunc(ident.getValue(), funcType.getType() == VOIDTK)) {
        errorList.emplace_back(Exception::REDEFINED_IDENT, ident.getLineNum());
    }
}
