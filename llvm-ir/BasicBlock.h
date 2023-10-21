//
// Created by hiccup on 2023/10/20.
//

#ifndef COMPILER_LLVM_IR_BASICBLOCK_H
#define COMPILER_LLVM_IR_BASICBLOCK_H

#include <list>
#include "instruction/Instruction.h"
#include "Function.h"

class BasicBlock : public Value {
	std::list<Instruction *> instructionList;
	Function *parent;
};

#endif //COMPILER_LLVM_IR_BASICBLOCK_H
