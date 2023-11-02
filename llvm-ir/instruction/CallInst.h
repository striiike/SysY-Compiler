//
// Created by hiccup on 2023/10/31.
//

#ifndef COMPILER_LLVM_IR_INSTRUCTION_CALLINST_H
#define COMPILER_LLVM_IR_INSTRUCTION_CALLINST_H

#include "Instruction.h"
#include "../Function.h"

#include <utility>
#include <vector>
#include <sstream>

class CallInst : public Instruction {
public:

	/// <result> = call <type> <function> (...)
	CallInst(std::string name, Function *func, std::vector<Value *> &args)
		: Instruction(func->getRetType(), std::move(name), InstType::CALL) {
		addOperand(func);
		for (auto i : args)
			addOperand(i);
	}

	std::string toString() override {
		std::stringstream ss;
		if (getType() != IntegerType::VOID) {
			ss << getName() + " = ";
		}
		ss << "call " + getType()->toString() + " " + getOperand(0)->getName();
		ss << "(";
		if (operandList.size() > 1) {
			ss << operandList[1]->toLlvmString();

			for (int i = 2; i < operandList.size(); ++i) {
				ss << ", " << operandList[i]->toLlvmString();
			}
		}
		ss << ")";
		return ss.str();
	}
};

#endif //COMPILER_LLVM_IR_INSTRUCTION_CALLINST_H
