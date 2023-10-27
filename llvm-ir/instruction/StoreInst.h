//
// Created by hiccup on 2023/10/27.
//

#ifndef COMPILER_LLVM_IR_INSTRUCTION_STOREINST_H
#define COMPILER_LLVM_IR_INSTRUCTION_STOREINST_H

#include "Instruction.h"
class StoreInst : public Instruction {

public:
	/// store <content> <addr>
	/// return void
	StoreInst(Value *content, Value *addr)
		: Instruction(IntegerType::VOID, "store", STORE) {
		this->addOperand(content);
		this->addOperand(addr);
	}

	std::string toString() override {
		return "store " + getOperand(0)->toString() + " " + getOperand(1)->toString();
	}
};

#endif //COMPILER_LLVM_IR_INSTRUCTION_STOREINST_H
