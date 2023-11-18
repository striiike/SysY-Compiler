//
// Created by hiccup on 2023/9/21.
//

#pragma once

#include <vector>
#include "../lexer/Lexer.h"
#include "ASTNode.h"
#include "UnaryExp.h"
#include "../../llvm-ir/IrBuilder.h"

using namespace std;

// only for certain syntax as (MulExp, AddExp, RelExp, EqExp, LAndExp, LOrExp)
//
//    <MulExp>        := <UnaryExp> { ('*' | '/' | '%') <UnaryExp> }
//    <AddExp>        := <MulExp> { ('+' | '-') <MulExp> }
//    <RelExp>        := <AddExp> { ('<' | '>' | '<=' | '>=') <AddExp> }
//    <EqExp>         := <RelExp> { ('==' | '!=') <RelExp> }
//    <LAndExp>       := <EqExp> { '&&' <EqExp> }
//    <LOrExp>        := <LAndExp> { '||' <LAndExp> }
//


template<typename T>
class ExpUniform : public ASTNode {
protected:
	T leftOperand;
	vector<TokenType> operators;
	vector<T> operands;
public:
	ExpUniform(T leftOperand, vector<TokenType> operators, vector<T> operands)
		: leftOperand(std::move(leftOperand)),
		  operators(std::move(operators)),
		  operands(std::move(operands)) {}

	T getLeftOperand() {
		return leftOperand;
	};

	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;

	// only for AddExp and MulExp
	int evaluate();

};
template<typename T>
int ExpUniform<T>::evaluate() {
	int res = leftOperand->evaluate();
	for (int i = 0; i < operands.size(); ++i) {
		switch (operators[i]) {
		case PLUS: res += operands[i]->evaluate();
			break;
		case MINU: res -= operands[i]->evaluate();
			break;
		case MULT: res *= operands[i]->evaluate();
			break;
		case DIV: res /= operands[i]->evaluate();
			break;
		case MOD: res %= operands[i]->evaluate();
			break;
		default: res = 1145141919;
			break;
		}
	}
	return res;
}
template<typename T>
void ExpUniform<T>::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	vector<int> dims{};
	auto _ret = make_shared<ErrorRet>();
	leftOperand->checkError(ctx, _ret);
	dims.push_back(_ret->dim);
	for (const auto &i : operands) {
		_ret = make_shared<ErrorRet>();
		i->checkError(ctx, _ret);
		dims.push_back(_ret->dim);
	}
	ret->dim = *min_element(dims.begin(), dims.end());
}

class MulExp : public ExpUniform<UnaryExpPtr> {
public:
	MulExp(UnaryExpPtr leftOperand, vector<TokenType> operators,
		   vector<UnaryExpPtr> operands);

	Value *llvmIr() override;
};

class AddExp : public ExpUniform<MulExpPtr> {
public:
	AddExp(MulExpPtr leftOperand, vector<TokenType> operators,
		   vector<MulExpPtr> operands);

	Value *llvmIr() override;
};

class RelExp : public ExpUniform<AddExpPtr> {
public:
	RelExp(AddExpPtr leftOperand, vector<TokenType> operators,
		   vector<AddExpPtr> operands);

	Value *llvmIr() override;
};

class EqExp : public ExpUniform<RelExpPtr> {
public:
	EqExp(RelExpPtr leftOperand, vector<TokenType> operators, vector<RelExpPtr> operands);

	Value *llvmIr() override;
};

class LAndExp : public ExpUniform<EqExpPtr> {
public:
	LAndExp(EqExpPtr leftOperand, vector<TokenType> operators,
			vector<EqExpPtr> operands);

	/// it needs a @exit label & a @failure label
	/// @failure is next LOrExp entry
	/// @exit
	Value *llvmIrAnd(BasicBlock *failure);
};

class LOrExp : public ExpUniform<LAndExpPtr> {
public:
	LOrExp(LAndExpPtr leftOperand, vector<TokenType> operators,
		   vector<LAndExpPtr> operands);

	/// it needs a @success label & a @next label
	/// @success is @thenBb
	/// @exit 	 is @elseBb or @follow
	Value *llvmIr() override;
};
