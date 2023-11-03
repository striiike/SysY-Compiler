#include "Exp.h"
#include "../parser/Symbol.hpp"

Number::Number(std::string num) :
	num(std::stoi(num)) {
	name = "<Number>";
	print();
}

void Number::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	ret->dim = 0;
}

int Number::evaluate() {
	return num;
}

Value *Number::llvmIr() {
	return new ConstantInt(num);
}

Exp::Exp(AddExpPtr sharedPtr, bool isConst) :
	addExpPtr(std::move(sharedPtr)), isConst(isConst) {
	name = isConst ? "<ConstExp>" : "<Exp>";
	print();
}

void Exp::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	addExpPtr->checkError(ctx, ret);
}

Value *Exp::llvmIr() {
	return addExpPtr->llvmIr();
}

// when computing global variable
int Exp::evaluate() {
	return addExpPtr->evaluate();
}

Cond::Cond(LOrExpPtr lOrExpPtr) : lOrExpPtr(std::move(lOrExpPtr)) {
	name = "<Cond>";
	print();
}

void Cond::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	lOrExpPtr->checkError(ctx, ret);
}

Value *Cond::llvmIr() {
	return lOrExpPtr->llvmIr();
}
