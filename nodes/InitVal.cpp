#include "InitVal.h"
#include "Exp.h"

ExpInitVal::ExpInitVal(bool isConst, ExpPtr expPtr) :
	isConst(isConst), expPtr(std::move(expPtr)) {
	name = isConst ? "<ConstInitVal>" : "<InitVal>";
	print();
}

void ExpInitVal::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	expPtr->checkError(ctx, ret);
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
