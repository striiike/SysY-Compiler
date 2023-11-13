#include "../nodes-include/InitVal.h"
#include "../nodes-include/Exp.h"

ExpInitVal::ExpInitVal(bool isConst, ExpPtr expPtr) :
	isConst(isConst), expPtr(std::move(expPtr)) {
	name = isConst ? "<ConstInitVal>" : "<InitVal>";
	print();
}

void ExpInitVal::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	expPtr->checkError(ctx, ret);
}

std::vector<int> ExpInitVal::evaluate() {
	return vector<int>{expPtr->evaluate()};
}
Value *ExpInitVal::llvmIr() {
	return expPtr->llvmIr();
}

ArrayInitVal::ArrayInitVal(bool isConst, std::vector<InitValPtr> initValPtrs) :
	isConst(isConst), initValPtrs(std::move(initValPtrs)) {
	name = isConst ? "<ConstInitVal>" : "<InitVal>";
	print();
}

void ArrayInitVal::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	for (const auto &i : initValPtrs) {
		i->checkError(ctx, ret);
	}
}

std::vector<int> ArrayInitVal::evaluate() {
	vector<int> values{};
	for (const auto &i : initValPtrs) {
		vector<int> _ = i->evaluate();
		values.insert(values.end(), _.begin(), _.end());
	}
	return values;
}
vector<Value *> ArrayInitVal::llvmIrList() {
	vector<Value *> irList{};
	irList.reserve(initValPtrs.size());
	for (const auto &i : initValPtrs) {
		if (i->isArray()) {
			auto res = i->llvmIrList();
			irList.insert(irList.end(), res.begin(), res.end());
		} else {
			irList.push_back(i->llvmIr());
		}
	}
	return irList;
}

