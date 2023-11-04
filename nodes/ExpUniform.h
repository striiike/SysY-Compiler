//
// Created by hiccup on 2023/9/21.
//

#pragma once

#include <vector>
#include <memory>
#include "../lexer/Lexer.h"
#include "ASTNode.h"
#include "UnaryExp.h"
#include <algorithm>

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

	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override {
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

	// only for AddExp and MulExp
	int evaluate() {
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

};

class MulExp : public ExpUniform<UnaryExpPtr> {
public:
	MulExp(UnaryExpPtr leftOperand, vector<TokenType> operators,
		   vector<UnaryExpPtr> operands);

	Value *llvmIr() override {
		auto *sum = leftOperand->llvmIr();
		for (int i = 0; i < operators.size(); ++i) {
			if (operators[i] == TokenType::MULT) {
				sum = irBuilder.buildAlu(AluType::MUL, sum, operands[i]->llvmIr());
			}
			if (operators[i] == TokenType::DIV) {
				sum = irBuilder.buildAlu(AluType::SDIV, sum, operands[i]->llvmIr());
			}
			if (operators[i] == TokenType::MOD) {
				sum = irBuilder.buildAlu(AluType::SREM, sum, operands[i]->llvmIr());
			}
		}
		return sum;
	}
};

class AddExp : public ExpUniform<MulExpPtr> {
public:
	AddExp(MulExpPtr leftOperand, vector<TokenType> operators,
		   vector<MulExpPtr> operands);

	Value *llvmIr() override {
		auto *sum = leftOperand->llvmIr();
		for (int i = 0; i < operators.size(); ++i) {
			if (operators[i] == TokenType::PLUS) {
				sum = irBuilder.buildAlu(AluType::ADD, sum, operands[i]->llvmIr());
			}
			if (operators[i] == TokenType::MINU) {
				sum = irBuilder.buildAlu(AluType::SUB, sum, operands[i]->llvmIr());
			}
		}
		return sum;
	}
};

class RelExp : public ExpUniform<AddExpPtr> {
public:
	RelExp(AddExpPtr leftOperand, vector<TokenType> operators,
		   vector<AddExpPtr> operands);

	Value *llvmIr() override {
		Value *val = leftOperand->llvmIr();
		for (int i = 0; i < operators.size(); ++i) {
			if (val->getType()->isInt1())
				val = irBuilder.buildZext(val, IntegerType::INT32);
			Value *cmpVal = operands[i]->llvmIr();
			if (cmpVal->getType()->isInt1())
				cmpVal = irBuilder.buildZext(cmpVal, IntegerType::INT32);
			if (operators[i] == TokenType::LSS) {
				val = irBuilder.buildIcmpInst(IcmpType::LT, val, cmpVal);
			}
			if (operators[i] == TokenType::LEQ) {
				val = irBuilder.buildIcmpInst(IcmpType::LE, val, cmpVal);
			}
			if (operators[i] == TokenType::GRE) {
				val = irBuilder.buildIcmpInst(IcmpType::GT, val, cmpVal);
			}
			if (operators[i] == TokenType::GEQ) {
				val = irBuilder.buildIcmpInst(IcmpType::GE, val, cmpVal);
			}
		}
		return val;
	}
};

class EqExp : public ExpUniform<RelExpPtr> {
public:
	EqExp(RelExpPtr leftOperand, vector<TokenType> operators, vector<RelExpPtr> operands);

	Value *llvmIr() override {
		if (operands.empty()) {
			Value *val = leftOperand->llvmIr();
			if (val->getType()->isInt32()) {
				val = irBuilder.buildIcmpInst(IcmpType::NE, val, new ConstantInt(0));
			}
			return val;
		} else {
			Value *val = leftOperand->llvmIr();
			for (int i = 0; i < operators.size(); ++i) {
				if (val->getType()->isInt1())
					val = irBuilder.buildZext(val, IntegerType::INT32);
				Value *cmpVal = operands[i]->llvmIr();
				if (cmpVal->getType()->isInt1())
					cmpVal = irBuilder.buildZext(cmpVal, IntegerType::INT32);
				if (operators[i] == TokenType::EQL) {
					val = irBuilder.buildIcmpInst(IcmpType::EQ, val, cmpVal);
				}
				if (operators[i] == TokenType::NEQ) {
					val = irBuilder.buildIcmpInst(IcmpType::NE, val, cmpVal);
				}
			}
			return val;
		}
	}
};

class LAndExp : public ExpUniform<EqExpPtr> {
public:
	LAndExp(EqExpPtr leftOperand, vector<TokenType> operators,
			vector<EqExpPtr> operands);

	/// it needs a @exit label & a @failure label
	/// @failure is next LOrExp entry
	/// @exit
	Value *llvmIrAnd(BasicBlock *failure) {
		if (operands.empty()) {
			auto *val = leftOperand->llvmIr();
			irBuilder.buildBrInst(val, irBuilder.ctx.thenBb, failure);
		} else {
			auto *val = leftOperand->llvmIr();
			BasicBlock *bb = irBuilder.buildBb();
			irBuilder.buildBrInst(val, bb, failure);
			irBuilder.setCurBb(bb);
			irBuilder.addBasicBlock(bb);

			int len = (int)operators.size();
			for (int i = 0; i < len; ++i) {
				if (i == len - 1) {
					val = leftOperand->llvmIr();
					irBuilder.buildBrInst(val, irBuilder.ctx.thenBb, failure);
				} else {
					val = leftOperand->llvmIr();
					bb = irBuilder.buildBb();
					irBuilder.buildBrInst(val, bb, failure);
					irBuilder.setCurBb(bb);
					irBuilder.addBasicBlock(bb);
				}
			}
		}

		return nullptr;
	}
};

class LOrExp : public ExpUniform<LAndExpPtr> {
public:
	LOrExp(LAndExpPtr leftOperand, vector<TokenType> operators,
		   vector<LAndExpPtr> operands);

	/// it needs a @success label & a @next label
	/// @success is @thenBb
	/// @exit 	 is @elseBb or @follow
	Value *llvmIr() override {
		if (operands.empty()) {
			leftOperand->llvmIrAnd(irBuilder.ctx.endBb);

		} else {
			BasicBlock *bb = irBuilder.buildBb();
			leftOperand->llvmIrAnd(bb);
			irBuilder.setCurBb(bb);
			irBuilder.addBasicBlock(bb);

			int len = (int)operators.size();
			for (int i = 0; i < len; ++i) {
				if (i == len - 1) {
					operands[i]->llvmIrAnd(irBuilder.ctx.endBb);
				} else {
					bb = irBuilder.buildBb();
					operands[i]->llvmIrAnd(bb);
					irBuilder.setCurBb(bb);
					irBuilder.addBasicBlock(bb);
				}
			}

		}

		return nullptr;
	}
};
