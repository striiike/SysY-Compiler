//
// Created by hiccup on 2023/10/27.
//

#ifndef COMPILER_LLVM_IR_INSTRUCTION_GEPINST_H
#define COMPILER_LLVM_IR_INSTRUCTION_GEPINST_H

#include <utility>

#include "Instruction.h"
class GEPInst : public Instruction {
	/// this is for what after inbounds
	Type *targetType;
public:

	/// @base is pointer, @ptrOff and @arrOff are index
	/// the instruction's type is @pointer to the @pointee of base
	/// normally is int32, because I flatten all the high level arrays.

	/// for example
	/// %13 = getelementptr inbounds [40 x i32], [40 x i32]* %12, i64 0, i64 1
	GEPInst(std::string name, Value *base, Value *ptrOff, Value *arrOff)
		: Instruction(
		new PointerType(base->getType()->getTargetType()->getEleType()),
		std::move(name), GEP) {

		this->targetType = base->getType()->getTargetType();
		this->addOperand(base);
		this->addOperand(ptrOff);
		this->addOperand(arrOff);
	}

	std::string toString() override {
		return name + " = getelementptr inbounds " +
			targetType->toString() + ", " +
			getOperand(0)->getType()->toString() + " " + getOperand(0)->getName() + ", " +
			getOperand(1)->getType()->toString() + " " + getOperand(1)->getName() + ", " +
			getOperand(2)->getType()->toString() + " " + getOperand(2)->getName();
	}
};

#endif //COMPILER_LLVM_IR_INSTRUCTION_GEPINST_H
