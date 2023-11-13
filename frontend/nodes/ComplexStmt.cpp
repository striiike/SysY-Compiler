#include "../nodes-include/ComplexStmt.h"
#include "../nodes-include/SimpleStmt.h"
#include "../nodes-include/LVal.h"

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

Value *_ForStmt::llvmIr() {
	auto *rvalue = expPtr->llvmIr();
	auto *lval = lValPtr->llvmIrAddr();
	irBuilder.buildStore(rvalue, lval);
	/// no need for sysY
	return nullptr;
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
	if (assignStmtPtr1)
		assignStmtPtr1->llvmIr();

	BasicBlock *condBb, *bodyBb, *endBb, *incBb;
	condBb = irBuilder.buildBb();
	irBuilder.buildBrInst(condBb);

	irBuilder.setCurBb(condBb);
	irBuilder.addBasicBlock(condBb);

	bodyBb = irBuilder.buildBb();
	incBb = irBuilder.buildBb();
	endBb = irBuilder.buildBb();

	irBuilder.incStack.push(incBb);
	irBuilder.endStack.push(endBb);
	irBuilder.ctx.condBb = condBb;
	irBuilder.ctx.thenBb = bodyBb;
	irBuilder.ctx.endBb = endBb;

	if (condPtr)
		condPtr->llvmIr();
	else
		irBuilder.buildBrInst(bodyBb);

	irBuilder.setCurBb(bodyBb);
	irBuilder.addBasicBlock(bodyBb);
	if (stmtPtr)
		stmtPtr->llvmIr();

	irBuilder.buildBrInst(incBb);
	irBuilder.setCurBb(incBb);
	irBuilder.addBasicBlock(incBb);

	if (assignStmtPtr2)
		assignStmtPtr2->llvmIr();
	irBuilder.buildBrInst(condBb);

	irBuilder.setCurBb(endBb);
	irBuilder.addBasicBlock(endBb);

	irBuilder.incStack.pop();
	irBuilder.endStack.pop();
	irBuilder.ctx.thenBb = nullptr;
	irBuilder.ctx.condBb = nullptr;
	irBuilder.ctx.endBb = nullptr;

	return nullptr;
}
