#include "Decl.h"

Decl::Decl(bool isConst, TokenNode bType, std::vector<DefPtr> defPtrs) :
	isConst(isConst),
	bType(std::move(bType)),
	defPtrs(std::move(defPtrs)) {
	name = isConst ? "<ConstDecl>" : "<VarDecl>";
	print();
}

void Decl::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	ctx->isConst = this->isConst;
	for (const auto &i : defPtrs)
		i->checkError(ctx, ret);
	ctx->isConst = false;
}

Value *Decl::llvmIr() {
	irBuilder.ctx.isConst = this->isConst;
	for (const auto &i : defPtrs)
		i->llvmIr();
	irBuilder.ctx.isConst = false;
	return nullptr;
}
