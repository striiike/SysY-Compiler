//
// Created by hiccup on 2023/9/27.
//

#pragma once

#include "ASTNode.h"

class FuncFParams : public ASTNode {
private:
	std::vector<FuncFParamPtr> funcFParamPtrs;
public:
	FuncFParams(std::vector<FuncFParamPtr> funcFParamPtrs) :
		funcFParamPtrs(std::move(funcFParamPtrs)) {
		name = "<FuncFParams>";
		print();
	}

	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;

	vector<int> getDims();
	Value *llvmIr() override;
};



