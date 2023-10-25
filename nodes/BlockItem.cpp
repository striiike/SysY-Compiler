#include "BlockItem.h"

BlockItem::BlockItem(DeclPtr declPtr, StmtPtr stmtPtr) :
	declPtr(std::move(declPtr)),
	stmtPtr(std::move(stmtPtr)) {
	name = "<BlockItem>";
}

void BlockItem::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	if (declPtr)
		declPtr->checkError(ctx, ret);
	if (stmtPtr)
		stmtPtr->checkError(ctx, ret);
}
void BlockItem::llvmIr() {
	if (declPtr)
		declPtr->llvmIr();
	if (stmtPtr)
		stmtPtr->llvmIr();
}
