//
// Created by hiccup on 2023/11/11.
//

#ifndef COMPILER_LLVM_IR_INSTRUCTION_PHIINST_H
#define COMPILER_LLVM_IR_INSTRUCTION_PHIINST_H

#include "Instruction.h"
#include <utility>
#include <vector>
#include <sstream>

class BasicBlock;

class PhiInst : public Instruction {
public:
	std::vector<BasicBlock *> *preBbs;
public:
	PhiInst(std::vector<BasicBlock *> *pre, std::string name)
		: Instruction(IntegerType::INT32, std::move(name), InstType::PHI) {
		preBbs = pre;
		operandList = std::vector<Value *>(pre->size());
	}

	void addValueFromBb(Value *val, BasicBlock *bb) {
		size_t index = std::find(preBbs->begin(), preBbs->end(), bb) - preBbs->begin();
		operandList[index] = val;
		useList.push_back(new Use(this, val));
	}

	std::string toString() override {
		std::stringstream ss;
		ss << name + " = phi " + type->toString() + " ";
		for (int i = 0; i < preBbs->size(); ++i) {
			ss << "[ " + operandList[i]->name + ", %" + (*preBbs)[i]->name + " ]";
			if (i!=preBbs->size() - 1)
				ss << ", ";
		}
		return ss.str();
	}

};

#endif //COMPILER_LLVM_IR_INSTRUCTION_PHIINST_H
