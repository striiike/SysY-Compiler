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
		this->addOp(content);
		this->addOp(addr);
	}

	std::string toString() override {
		return "store " +
			getOp(0)->toLlvmString() + ", " +
			getOp(1)->toLlvmString();
	}
};

#endif //COMPILER_LLVM_IR_INSTRUCTION_STOREINST_H
