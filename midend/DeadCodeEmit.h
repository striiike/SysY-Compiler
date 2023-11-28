//
// Created by hiccup on 2023/11/28.
//

#ifndef COMPILER_MIDEND_DEADCODEEMIT_H
#define COMPILER_MIDEND_DEADCODEEMIT_H

#include <unordered_set>
#include "../llvm-ir/instruction/Instruction.h"
#include "../llvm-ir/Module.h"

class DeadCodeEmit {
public:
	std::unordered_set<Instruction *> usefulInsts;

	void instClosure(Instruction *i);
	void run(Module *module);
};

#endif //COMPILER_MIDEND_DEADCODEEMIT_H
