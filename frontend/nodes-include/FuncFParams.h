//
// Created by hiccup on 2023/9/27.
//

#pragma once

#include <utility>
#include "ASTNode.h"

class FuncFParams : public ASTNode {
private:
	std::vector<FuncFParamPtr> funcFParamPtrs;
public:
	explicit FuncFParams(std::vector<FuncFParamPtr> funcFParamPtrs);

	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;

	vector<int> getDims();
	Value *llvmIr() override;
};



