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
		addOp(func);
		for (auto i : args)
			addOp(i);
	}

	std::string toString() override {
		std::stringstream ss;
		if (type!=IntegerType::VOID) {
			ss << name + " = ";
		}
		ss << "call " + type->toString() + " " + getOp(0)->name;
		ss << "(";
		if (opList.size() > 1) {
			ss << opList[1]->toLlvmString();

			for (int i = 2; i < opList.size(); ++i) {
				ss << ", " << opList[i]->toLlvmString();
			}
		}
		ss << ")";
		return ss.str();
	}
};

#endif //COMPILER_LLVM_IR_INSTRUCTION_CALLINST_H
