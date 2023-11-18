#include "../nodes-include/ExpUniform.h"

MulExp::MulExp(UnaryExpPtr leftOperand, vector<TokenType> operators, vector<UnaryExpPtr> operands)
	: ExpUniform<UnaryExpPtr>(std::move(leftOperand), std::move(operators), std::move(operands)) {
	name = "<MulExp>";
	print();
}
Value *MulExp::llvmIr() {
	auto *sum = leftOperand->llvmIr();
	for (int i = 0; i < operators.size(); ++i) {
		if (operators[i]==TokenType::MULT) {
			sum = irBuilder.buildAlu(AluType::MUL, sum, operands[i]->llvmIr());
		}
		if (operators[i]==TokenType::DIV) {
			sum = irBuilder.buildAlu(AluType::SDIV, sum, operands[i]->llvmIr());
		}
		if (operators[i]==TokenType::MOD) {
			sum = irBuilder.buildAlu(AluType::SREM, sum, operands[i]->llvmIr());
		}
	}
	return sum;
}

AddExp::AddExp(MulExpPtr leftOperand, vector<TokenType> operators, vector<MulExpPtr> operands)
	: ExpUniform<MulExpPtr>(std::move(leftOperand), std::move(operators), std::move(operands)) {
	name = "<AddExp>";
	print();
}
Value *AddExp::llvmIr() {
	auto *sum = leftOperand->llvmIr();
	for (int i = 0; i < operators.size(); ++i) {
		if (operators[i]==TokenType::PLUS) {
			sum = irBuilder.buildAlu(AluType::ADD, sum, operands[i]->llvmIr());
		}
		if (operators[i]==TokenType::MINU) {
			sum = irBuilder.buildAlu(AluType::SUB, sum, operands[i]->llvmIr());
		}
	}
	return sum;
}

RelExp::RelExp(AddExpPtr leftOperand, vector<TokenType> operators, vector<AddExpPtr> operands)
	: ExpUniform<AddExpPtr>(std::move(leftOperand), std::move(operators), std::move(operands)) {
	name = "<RelExp>";
	print();
}
Value *RelExp::llvmIr() {
	Value *val = leftOperand->llvmIr();
	for (int i = 0; i < operators.size(); ++i) {
		if (val->type->isInt1())
			val = irBuilder.buildZext(val, IntegerType::INT32);
		Value *cmpVal = operands[i]->llvmIr();
		if (cmpVal->type->isInt1())
			cmpVal = irBuilder.buildZext(cmpVal, IntegerType::INT32);
		if (operators[i]==TokenType::LSS) {
			val = irBuilder.buildIcmpInst(IcmpType::LT, val, cmpVal);
		}
		if (operators[i]==TokenType::LEQ) {
			val = irBuilder.buildIcmpInst(IcmpType::LE, val, cmpVal);
		}
		if (operators[i]==TokenType::GRE) {
			val = irBuilder.buildIcmpInst(IcmpType::GT, val, cmpVal);
		}
		if (operators[i]==TokenType::GEQ) {
			val = irBuilder.buildIcmpInst(IcmpType::GE, val, cmpVal);
		}
	}
	return val;
}

LAndExp::LAndExp(EqExpPtr leftOperand, vector<TokenType> operators, vector<EqExpPtr> operands)
	: ExpUniform<EqExpPtr>(std::move(leftOperand), std::move(operators), std::move(operands)) {
	name = "<LAndExp>";
	print();
}
Value *LAndExp::llvmIrAnd(BasicBlock *failure) {
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
			if (i==len - 1) {
				val = operands[i]->llvmIr();
				irBuilder.buildBrInst(val, irBuilder.ctx.thenBb, failure);
			} else {
				val = operands[i]->llvmIr();
				bb = irBuilder.buildBb();
				irBuilder.buildBrInst(val, bb, failure);
				irBuilder.setCurBb(bb);
				irBuilder.addBasicBlock(bb);
			}
		}
	}

	return nullptr;
}

LOrExp::LOrExp(LAndExpPtr leftOperand, vector<TokenType> operators, vector<LAndExpPtr> operands)
	: ExpUniform<LAndExpPtr>(std::move(leftOperand), std::move(operators), std::move(operands)) {
	name = "<LOrExp>";
	print();
}
Value *LOrExp::llvmIr() {
	if (operands.empty()) {
		leftOperand->llvmIrAnd(irBuilder.ctx.endBb);

	} else {
		BasicBlock *bb = irBuilder.buildBb();
		leftOperand->llvmIrAnd(bb);
		irBuilder.setCurBb(bb);
		irBuilder.addBasicBlock(bb);

		int len = (int)operators.size();
		for (int i = 0; i < len; ++i) {
			if (i==len - 1) {
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

EqExp::EqExp(RelExpPtr leftOperand, vector<TokenType> operators, vector<RelExpPtr> operands)
	: ExpUniform<RelExpPtr>(std::move(leftOperand), std::move(operators), std::move(operands)) {
	name = "<EqExp>";
	print();
}
Value *EqExp::llvmIr() {
	if (operands.empty()) {
		Value *val = leftOperand->llvmIr();
		if (val->type->isInt32()) {
			val = irBuilder.buildIcmpInst(IcmpType::NE, val, new ConstantInt(0));
		}
		return val;
	} else {
		Value *val = leftOperand->llvmIr();
		for (int i = 0; i < operators.size(); ++i) {
			if (val->type->isInt1())
				val = irBuilder.buildZext(val, IntegerType::INT32);
			Value *cmpVal = operands[i]->llvmIr();
			if (cmpVal->type->isInt1())
				cmpVal = irBuilder.buildZext(cmpVal, IntegerType::INT32);
			if (operators[i]==TokenType::EQL) {
				val = irBuilder.buildIcmpInst(IcmpType::EQ, val, cmpVal);
			}
			if (operators[i]==TokenType::NEQ) {
				val = irBuilder.buildIcmpInst(IcmpType::NE, val, cmpVal);
			}
		}
		return val;
	}
}
