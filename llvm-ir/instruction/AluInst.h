//
// Created by hiccup on 2023/10/26.
//

#ifndef COMPILER_LLVM_IR_INSTRUCTION_ALUINST_H
#define COMPILER_LLVM_IR_INSTRUCTION_ALUINST_H

#include "Instruction.h"

enum AluType {
	ADD, SUB, MUL, SDIV, ICMP
};

class AluInst : public Instruction {
	AluType aluType;
public:
	//	add 	<result> = add <ty> <op1>, <op2>
	//	sub		<result> = sub <ty> <op1>, <op2>
	//	mul		<result> = mul <ty> <op1>, <op2>
	//	sdiv	<result> = sdiv <ty> <op1>, <op2>			signed div
	//	icmp	<result> = icmp <cond> <ty> <op1>, <op2>
	//	and		<result> = and <ty> <op1>, <op2>
	//	or		<result> = or <ty> <op1>, <op2>
	AluInst() = default;
};

#endif //COMPILER_LLVM_IR_INSTRUCTION_ALUINST_H
