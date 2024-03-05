//
// Created by hiccup on 2023/11/30.
//

#ifndef COMPILER_BACKEND_PEEPHOLE_H
#define COMPILER_BACKEND_PEEPHOLE_H

#include "mips/MipsComponent.h"
class PeepHole {
	MipsModule *module;

public:

	void run(MipsModule *module);
	bool imm2Zero(_List_iterator<MipsInst *> it);
	bool imm2Zero(_List_iterator<MipsInst *> it, MipsBlock *bb);
};

#endif //COMPILER_BACKEND_PEEPHOLE_H
