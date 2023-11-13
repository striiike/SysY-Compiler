//
// Created by hiccup on 2023/11/9.
//

#ifndef COMPILER_MIDEND_MIDEND_H
#define COMPILER_MIDEND_MIDEND_H

#include "DFBuilder.h"
#include "../llvm-ir/Module.h"
#include "Mem2Reg.h"

class MidEnd {
public:
	void run(Module *module) {
		(new DFBuilder(&(module->functionList)))->run();

		(new Mem2Reg())->run(module);
	}
};

#endif //COMPILER_MIDEND_MIDEND_H
