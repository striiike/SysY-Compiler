#include "ComplexStmt.h"
#include "SimpleStmt.h"

IfStmt::IfStmt(CondPtr condPtr, StmtPtr stmtPtr, StmtPtr stmtElsePtr) :
        condPtr(std::move(condPtr)), stmtPtr(std::move(stmtPtr)),
        stmtElsePtr(std::move(stmtElsePtr)), ComplexStmt() {}

void IfStmt::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
    condPtr->checkError(ctx, ret);
    stmtPtr->checkError(ctx, ret);
    if (stmtElsePtr) stmtElsePtr->checkError(ctx, ret);
}

ForStmt::ForStmt(CondPtr condPtr, StmtPtr stmtPtr, _ForStmtPtr assignStmtPtr1, _ForStmtPtr assignStmtPtr2) :
        condPtr(std::move(condPtr)), stmtPtr(std::move(stmtPtr)),
        assignStmtPtr1(std::move(assignStmtPtr1)),
        assignStmtPtr2(std::move(assignStmtPtr2)), ComplexStmt() {}

void ForStmt::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
    if (assignStmtPtr1) assignStmtPtr1->checkError(ctx, ret);
    if (condPtr) condPtr->checkError(ctx, ret);
    if (assignStmtPtr2) assignStmtPtr2->checkError(ctx, ret);

    ctx->layerNum++;
    stmtPtr->checkError(ctx, ret);
    ctx->layerNum--;
}

Block::Block(std::vector<BlockItemPtr> blockItemPtrs, TokenNode rbrace) :
        blockItemPtrs(std::move(blockItemPtrs)), rbrace(std::move(rbrace)) {
    name = "<Block>";
    print();
}

void Block::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
    symbol.startScope();
    for (const auto& i : blockItemPtrs) {
        i->checkError(ctx, ret);
    }
    symbol.endScope();

    if (ctx->inFunc) {
        if (!ctx->voidFunc && !ret->hasRet) {
            errorList.emplace_back(Exception::INT_RETURN_LACKED, rbrace.getLineNum());
        }

        if (ctx->voidFunc && ret->hasRet) {
            errorList.emplace_back(Exception::VOID_RETURN_INT, ret->retLineNum);
        }
    }
}
