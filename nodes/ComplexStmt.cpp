#include "ComplexStmt.h"
#include "SimpleStmt.h"

IfStmt::IfStmt(CondPtr condPtr, StmtPtr stmtPtr, StmtPtr stmtElsePtr) :
	condPtr(std::move(condPtr)), stmtPtr(std::move(stmtPtr)),
	stmtElsePtr(std::move(stmtElsePtr)), ComplexStmt() {}

void IfStmt::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	condPtr->checkError(ctx, ret);
	stmtPtr->checkError(ctx, ret);
	if (stmtElsePtr)
		stmtElsePtr->checkError(ctx, ret);
}

ForStmt::ForStmt(CondPtr condPtr, StmtPtr stmtPtr, _ForStmtPtr assignStmtPtr1, _ForStmtPtr assignStmtPtr2) :
	condPtr(std::move(condPtr)), stmtPtr(std::move(stmtPtr)),
	assignStmtPtr1(std::move(assignStmtPtr1)),
	assignStmtPtr2(std::move(assignStmtPtr2)), ComplexStmt() {}

void ForStmt::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	if (assignStmtPtr1)
		assignStmtPtr1->checkError(ctx, ret);
	if (condPtr)
		condPtr->checkError(ctx, ret);
	if (assignStmtPtr2)
		assignStmtPtr2->checkError(ctx, ret);

	ctx->loopNum++;
	stmtPtr->checkError(ctx, ret);
	ctx->loopNum--;
}

Block::Block(std::vector<BlockItemPtr> blockItemPtrs, TokenNode rbrace) :
	blockItemPtrs(std::move(blockItemPtrs)), rbrace(std::move(rbrace)) {
	name = "<Block>";
	print();
}

void Block::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	if (!ctx->afterFuncDef)
		symbol.startScope();
	else
		ctx->afterFuncDef = false;

	ctx->layerNum++;
	for (const auto &i : blockItemPtrs) {
		i->checkError(ctx, ret);
	}
	ctx->layerNum--;

	symbol.endScope();
	ret->rbraceLineNum = rbrace.getLineNum();
}
