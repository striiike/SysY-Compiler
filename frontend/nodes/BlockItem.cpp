#include "../nodes-include/BlockItem.h"
#include "../nodes-include/Decl.h"
#include "../nodes-include/Stmt.h"

BlockItem::BlockItem(DeclPtr declPtr, StmtPtr stmtPtr) :
	declPtr(declPtr),
	stmtPtr(stmtPtr) {
	name = "<BlockItem>";
}

void BlockItem::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	if (declPtr)
		declPtr->checkError(ctx, ret);
	if (stmtPtr)
		stmtPtr->checkError(ctx, ret);
}

Value *BlockItem::llvmIr() {
	if (declPtr)
		declPtr->llvmIr();
	if (stmtPtr)
		stmtPtr->llvmIr();
	return nullptr;
}
