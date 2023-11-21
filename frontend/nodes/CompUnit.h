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

	CompUnit(std::vector<DeclPtr> declPtrs, std::vector<FuncDefPtr> funcDefPtrs, MainFuncDefPtr mainFuncDefPtr) :
		declPtrs(std::move(declPtrs)),
		funcDefPtrs(std::move(funcDefPtrs)),
		mainFuncDefPtr(mainFuncDefPtr) {
		name = "<CompUnit>";
		print();
	}
	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	Value *llvmIr() override;
};



