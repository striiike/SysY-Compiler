#include "ComplexStmt.h"
#include "SimpleStmt.h"
#include "LVal.h"

IfStmt::IfStmt(CondPtr condPtr, StmtPtr stmtPtr, StmtPtr stmtElsePtr) :
	condPtr(std::move(condPtr)), stmtPtr(std::move(stmtPtr)),
	stmtElsePtr(std::move(stmtElsePtr)), ComplexStmt() {}

void IfStmt::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	condPtr->checkError(ctx, ret);
	stmtPtr->checkError(ctx, ret);
	if (stmtElsePtr)
		stmtElsePtr->checkError(ctx, ret);
}

Value *IfStmt::llvmIr() {
	BasicBlock *thenBb, *elseBb, *endBb;
	thenBb = irBuilder.buildBb();
	if (stmtElsePtr)
		elseBb = irBuilder.buildBb();
	endBb = irBuilder.buildBb();

	irBuilder.ctx.thenBb = thenBb;
	irBuilder.ctx.endBb = (stmtElsePtr) ? elseBb : endBb;

	condPtr->llvmIr();

	irBuilder.setCurBb(thenBb);
	irBuilder.addBasicBlock(thenBb);
	stmtPtr->llvmIr();
	irBuilder.buildBrInst(endBb);

	if (stmtElsePtr) {
		irBuilder.setCurBb(elseBb);
		irBuilder.addBasicBlock(elseBb);
		stmtElsePtr->llvmIr();
		irBuilder.buildBrInst(endBb);
	}

	irBuilder.setCurBb(endBb);
	irBuilder.addBasicBlock(endBb);

	irBuilder.ctx.thenBb = nullptr;
	irBuilder.ctx.endBb = nullptr;

	return nullptr;
}

_ForStmt::_ForStmt(LValPtr lValPtr, ExpPtr expPtr)
	: lValPtr(std::move(lValPtr)), expPtr(std::move(expPtr)) {
	name = "<ForStmt>";
	print();
}

void _ForStmt::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	ctx->isLeftValue = true;
	lValPtr->checkError(ctx, ret);
	ctx->isLeftValue = false;
	expPtr->checkError(ctx, ret);
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
Value *ForStmt::llvmIr() {

}
