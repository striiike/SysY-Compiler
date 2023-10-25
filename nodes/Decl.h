//
// Created by hiccup on 2023/9/25.
//

#pragma once

#include <utility>
#include <vector>
#include "ASTNode.h"
#include "TokenNode.h"
#include "Def.h"

class Decl : public ASTNode {
	bool isConst;
	TokenNode bType;
	std::vector<DefPtr> defPtrs;
public :
	Decl(bool isConst, TokenNode bType, std::vector<DefPtr> defPtrs);

	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	void llvmIr() override;
};


