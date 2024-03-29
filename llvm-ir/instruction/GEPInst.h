//
// Created by hiccup on 2023/10/27.
//

#ifndef COMPILER_LLVM_IR_INSTRUCTION_GEPINST_H
#define COMPILER_LLVM_IR_INSTRUCTION_GEPINST_H

#include <utility>
#include <sstream>

#include "Instruction.h"
class GEPInst : public Instruction {
	/// this is for what after inbounds
	Type *targetType;
public:

	/// @base is pointer, @ptrOff and @arrOff are index
	/// the instruction's type is @pointer to the @pointee of base
	/// normally is int32, because I flatten all the high level arrays.

	/// for example
	/// %13 = getelementptr inbounds [40 x i32], [40 x i32]* %12, i32 0, i32 1
	GEPInst(std::string name, Value *base, Value *ptrOff, Value *arrOff)
		: Instruction(
		new PointerType(base->type->getTargetType()->getEleType()),
		std::move(name), GEP) {

		this->targetType = base->type->getTargetType();
		/// @@@@@@ @sad @@@@@@
		if (targetType==IntegerType::INT32)
			this->type = new PointerType(IntegerType::INT32);

		this->addOp(base);
		this->addOp(ptrOff);
		if (arrOff)
			this->addOp(arrOff);
	}

	std::string toString() override {
		std::stringstream ss;
		ss << name + " = getelementptr inbounds " +
			targetType->toString() + ", " +
			getOp(0)->toLlvmString() + ", " +
			getOp(1)->toLlvmString();

		if (getOp(2)) {
			ss << ", " + getOp(2)->toLlvmString();
		}
		return ss.str();
	}
};

#endif //COMPILER_LLVM_IR_INSTRUCTION_GEPINST_H
