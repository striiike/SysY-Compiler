//
// Created by hiccup on 2023/10/25.
//

#ifndef COMPILER_NODES_BLOCK_H
#define COMPILER_NODES_BLOCK_H

#include "ComplexStmt.h"

class Block : public ComplexStmt {
private:
	std::vector<BlockItemPtr> blockItemPtrs;
	TokenNode rbrace;
public:
	explicit Block(std::vector<BlockItemPtr> blockItemPtrs, TokenNode rbrace);

	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	Value *llvmIr() override;
};
#endif //COMPILER_NODES_BLOCK_H
