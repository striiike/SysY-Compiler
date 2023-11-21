//
// Created by hiccup on 2023/9/25.
//

#pragma once

#include "ASTNode.h"
#include "TokenNode.h"
#include "Def.h"

class Decl : public ASTNode {
	bool isConst;
	TokenNode bType;
	std::vector<DefPtr> defPtrs;
public :
	Decl(bool isConst, TokenNode bType, std::vector<DefPtr> defPtrs) :
		isConst(isConst),
		bType(std::move(bType)),
		defPtrs(std::move(defPtrs)) {
		name = isConst ? "<ConstDecl>" : "<VarDecl>";
		print();
	}


	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	Value *llvmIr() override;
};


