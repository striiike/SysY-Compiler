//
// Created by hiccup on 2023/10/26.
//

#ifndef COMPILER_LLVM_IR_INSTRUCTION_ALUINST_H
#define COMPILER_LLVM_IR_INSTRUCTION_ALUINST_H

#include <utility>
#include <map>

#include "Instruction.h"

enum AluType {
	ADD, SUB, MUL, SDIV, SREM
};

class AluInst : public Instruction {
public:
	AluType aluType;
public:
	std::map<AluType, std::string> aluTypeMap = {
		{AluType::ADD, "add"},
		{AluType::SUB, "sub"},
		{AluType::MUL, "mul"},
		{AluType::SDIV, "sdiv"},
		{AluType::SREM, "srem"}
	};
	//	add 	<result> = add <ty> <op1>, <op2>
	//	sub		<result> = sub <ty> <op1>, <op2>
	//	mul		<result> = mul <ty> <op1>, <op2>
	//	sdiv	<result> = sdiv <ty> <op1>, <op2>			signed div
	//	icmp	<result> = icmp <cond> <ty> <op1>, <op2>
	//	and		<result> = and <ty> <op1>, <op2>
	//	or		<result> = or <ty> <op1>, <op2>
	AluInst(std::string name, AluType ty, Value *op1, Value *op2)
		: Instruction(IntegerType::INT32, std::move(name), InstType::ALU),
		  aluType(ty) {
		addOperand(op1);
		addOperand(op2);
	}

	std::string toString() override {
		return name + " = " + aluTypeMap[aluType] + " " +
			getOperand(0)->toLlvmString() + ", " +
			getOperand(1)->getName();

	}
};

#endif //COMPILER_LLVM_IR_INSTRUCTION_ALUINST_H
