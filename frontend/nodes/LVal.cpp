//
// Created by hiccup on 2023/10/9.
//

#include "../nodes-include/LVal.h"
#include "../nodes-include/Exp.h"
#include "../parser/Symbol.hpp"
#include "../../llvm-ir/IrBuilder.h"

LVal::LVal(TokenNode ident, std::vector<ExpPtr> array) : ident(std::move(ident)), array(std::move(array)) {
	name = "<LVal>";
	print();
}

void LVal::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	auto varPtr = symbol.getVar(ident.getValue());
	vector<int> lens{};
	if (varPtr)
		lens = varPtr->lens;
	if (!varPtr) {
		errorList.emplace_back(Exception::UNDEFINED_IDENT, ident.getLineNum());
		ret->undefined = true;
	} else if (varPtr->isConst && ctx->isLeftValue)
		errorList.emplace_back(Exception::CONST_ASSIGNED, ident.getLineNum());
	for (const auto &i : array) {
		i->checkError(ctx, ret);
	}

	ret->dim = (int)lens.size() - (int)array.size();
}

int LVal::evaluate() {
	vector<int> index{};
	index.reserve(array.size());
	for (const auto &i : array) {
		index.push_back(i->evaluate());
	}
	auto varPtr = symbol.getVar(ident.getValue());
	int res = varPtr->getValue(index);
	return res;
}

Value *LVal::llvmIr() {

	auto varSymbol = symbol.getVar(ident.getValue());
	Value *val = varSymbol->getLlvmValue();
	bool isPointer = array.size()!=varSymbol->lens.size();

	/// single-var
	if (varSymbol->lens.empty()) {
		if (varSymbol->isConst)
			return new ConstantInt(varSymbol->getValue(vector<int>{}));
		else {
			auto *load = irBuilder.buildLoad(val);
			return load;
		}
	} else if (isPointer) {
		Value *realOff = new ConstantInt(0);
		if (varSymbol->lens.size()==2 && array.size()==1) {
			Value *arrOff = array[0]->llvmIr();
			realOff = irBuilder.buildAlu(AluType::MUL, arrOff, new ConstantInt(varSymbol->lens[1]));
		}
		auto *gep = irBuilder.buildGEP(varSymbol->getLlvmValue(), realOff);
		return gep;

	} else {
		Value *dim1 = array.back()->llvmIr();
		/// @manual for 2 dimension
		if (array.size()==2) {
			Value *mul = irBuilder.buildAlu(
				AluType::MUL,
				array[0]->llvmIr(),
				new ConstantInt(varSymbol->lens.back()));
			dim1 = irBuilder.buildAlu(AluType::ADD, mul, dim1);
		}
		auto *gep = irBuilder.buildGEP(val, dim1);
		auto *load = irBuilder.buildLoad(gep);
		return load;
	}

	return nullptr;
}

Value *LVal::llvmIrAddr() {
	auto varSymbol = symbol.getVar(ident.getValue());
	Value *addr = varSymbol->getLlvmValue();

	if (array.empty()) {

	} else {
		Value *dim1 = array.back()->llvmIr();
		/// @manual for 2 dimension
		if (array.size()==2) {
			Value *mul = irBuilder.buildAlu(
				AluType::MUL,
				array[0]->llvmIr(),
				new ConstantInt(varSymbol->lens.back()));
			dim1 = irBuilder.buildAlu(AluType::ADD, mul, dim1);
		}
		addr = irBuilder.buildGEP(addr, dim1);
	}
	return addr;
}