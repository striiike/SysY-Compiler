//
// Created by hiccup on 2023/10/20.
//

#ifndef COMPILER_LLVM_IR_INSTRUCTION_H
#define COMPILER_LLVM_IR_INSTRUCTION_H

#include "../BasicBlock.h"
#include "../User.h"


class Instruction : User{
	BasicBlock *parent;
};

#endif //COMPILER_LLVM_IR_INSTRUCTION_H
