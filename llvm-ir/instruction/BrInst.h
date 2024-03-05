//
// Created by hiccup on 2023/11/2.
//

#ifndef COMPILER_LLVM_IR_INSTRUCTION_BRINST_H
#define COMPILER_LLVM_IR_INSTRUCTION_BRINST_H

#include "Instruction.h"

class BrInst : public Instruction {
public:
	// jump -> br label %x
	// not jump -> br i1 %n label %x label %y
	bool jump;
public:
	BrInst(Value *condition, BasicBlock *bb1, BasicBlock *bb2)
		: Instruction(IntegerType::VOID, "br", InstType::BRANCH) {
		addOp(condition);
		addOp((Value *)bb1);
		addOp((Value *)bb2);
		jump = false;
	}

	explicit BrInst(BasicBlock *bb)
		: Instruction(IntegerType::VOID, "jump", InstType::BRANCH) {
		addOp((Value *)bb);
		jump = true;
	}

	std::string toString() override {
		if (jump) {
			return "br label %" + getOp(0)->name;
		} else {
			return "br " +
				getOp(0)->toLlvmString() + ", " +
				"label %" + getOp(1)->name + ", " +
				"label %" + getOp(2)->name;
		}

	}
};

#endif //COMPILER_LLVM_IR_INSTRUCTION_BRINST_H
