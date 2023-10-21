//
// Created by hiccup on 2023/9/27.
//

#pragma once

#include <utility>
#include <vector>
#include "ASTNode.h"

class CompUnit : public ASTNode {
	std::vector<DeclPtr> declPtrs;
	std::vector<FuncDefPtr> funcDefPtrs;
	MainFuncDefPtr mainFuncDefPtr;
public:
	explicit CompUnit(std::vector<DeclPtr> declPtrs, std::vector<FuncDefPtr> funcDefPtrs,
					  MainFuncDefPtr mainFuncDefPtr);

	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	void llvmIr();
};



