//
// Created by hiccup on 2023/10/20.
//

#ifndef COMPILER_LLVM_IR_BASICBLOCK_H
#define COMPILER_LLVM_IR_BASICBLOCK_H

#include <list>
#include <utility>
#include "instruction/Instruction.h"
//#include "Function.h"

class Function;


class BasicBlock : public Value {
	std::list<Instruction *> instructionList;
	Function *parent{};

public:
	explicit BasicBlock(std::string name)
		: Value(nullptr, std::move(name)) {}

	void setParent(Function *func) {
		parent = func;
	}

	void addInstruction(Instruction *inst) {
		instructionList.push_back(inst);
	}

};

#endif //COMPILER_LLVM_IR_BASICBLOCK_H
