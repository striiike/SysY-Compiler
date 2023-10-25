//
// Created by hiccup on 2023/10/20.
//

#ifndef COMPILER_LLVM_IR_INSTRUCTION_H
#define COMPILER_LLVM_IR_INSTRUCTION_H

#include "../BasicBlock.h"
#include "../User.h"

class BasicBlock;

class Instruction : public User{
	BasicBlock *parent{};
public:

};

#endif //COMPILER_LLVM_IR_INSTRUCTION_H
