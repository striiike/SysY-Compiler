//
// Created by hiccup on 2023/10/25.
//

#include "../nodes-include/Block.h"
#include "../parser/Symbol.hpp"
#include "../../llvm-ir/IrBuilder.h"

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

Value *Block::llvmIr() {
	if (!irBuilder.ctx.afterFuncDef)
		symbol.startScope();
	else
		irBuilder.ctx.afterFuncDef = false;

	for (const auto &i : blockItemPtrs) {
		i->llvmIr();
	}

	symbol.endScope();
	return nullptr;
}