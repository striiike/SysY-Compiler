//
// Created by hiccup on 2023/11/2.
//

#ifndef COMPILER_LLVM_IR_INSTRUCTION_ICMPINST_H
#define COMPILER_LLVM_IR_INSTRUCTION_ICMPINST_H

#include <utility>
#include <map>

#include "Instruction.h"
enum IcmpType {
	EQ, NE, LT, LE, GT, GE
};

class IcmpInst : public Instruction {
public:
	IcmpType icmpType;
public:
	std::map<IcmpType, std::string> icmpTypeMap = {
		{EQ, "eq"},
		{NE, "ne"},
		{LT, "slt"},
		{LE, "sle"},
		{GT, "sgt"},
		{GE, "sge"}
	};

	IcmpInst(std::string name, IcmpType ty, Value *op1, Value *op2)
		: Instruction(IntegerType::INT1, std::move(name), InstType::ICMP) {
		icmpType = ty;
		addOperand(op1);
		addOperand(op2);
	}

	std::string toString() override {
		return name + " = icmp " + icmpTypeMap[icmpType] + " " +
			getOperand(0)->toLlvmString() + ", " + getOperand(1)->name;
	}

};

#endif //COMPILER_LLVM_IR_INSTRUCTION_ICMPINST_H
