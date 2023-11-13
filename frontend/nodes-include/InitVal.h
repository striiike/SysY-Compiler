//
// Created by hiccup on 2023/9/25.
//

#pragma once

#include <utility>
#include <vector>
#include "ASTNode.h"

class InitVal : public ASTNode {
public:
	virtual void debug() {}
	virtual std::vector<int> evaluate() { return vector<int>{}; }
	virtual std::vector<Value *> llvmIrList() { return vector<Value *>{}; }
	virtual bool isArray() { return false; }
};

class ExpInitVal : public InitVal {
private:
	bool isConst;
	ExpPtr expPtr;
public:
	ExpInitVal(bool isConst, ExpPtr expPtr);

	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	std::vector<int> evaluate() override;
	Value *llvmIr() override;
};

class ArrayInitVal : public InitVal {
private:
	bool isConst;
	std::vector<InitValPtr> initValPtrs;
public:
	explicit ArrayInitVal(bool isConst, std::vector<InitValPtr> initValPtrs);

	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;
	std::vector<int> evaluate() override;
	vector<Value *> llvmIrList() override;
	bool isArray() override { return true; }
};

