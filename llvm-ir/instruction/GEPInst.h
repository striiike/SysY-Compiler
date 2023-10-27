//
// Created by hiccup on 2023/10/27.
//

#ifndef COMPILER_LLVM_IR_INSTRUCTION_GEPINST_H
#define COMPILER_LLVM_IR_INSTRUCTION_GEPINST_H

#include <utility>

#include "Instruction.h"
class GEPInst : public Instruction {
public:

	/// @base is pointer, @ptrOff and @arrOff are index
	/// the instruction's type is @pointer to the @pointee of base
	/// normally is int32, because I flatten all the high level arrays.

	/// for example
	/// %13 = getelementptr inbounds [40 x i32], [40 x i32]* %12, i64 0, i64 1
	GEPInst(std::string name, Value *base, Value *ptrOff, Value *arrOff)
		: Instruction(new PointerType(
						  static_cast<PointerType>((base->getType())).getTargetType()),
					  std::move(name),
					  GEP) {
		this->addOperand(base);
		this->addOperand(ptrOff);
		this->addOperand(arrOff);
	}

	std::string toString() override {
		return name + " = getelementptr inbounds " + " ???, " +
			getOperand(0)->toString() + ", " +
			getOperand(1)->toString() + ", " +
			getOperand(2)->toString();
	}
};

#endif //COMPILER_LLVM_IR_INSTRUCTION_GEPINST_H
