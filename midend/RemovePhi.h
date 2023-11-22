//
// Created by hiccup on 2023/11/22.
//

#ifndef COMPILER_MIDEND_REMOVEPHI_H
#define COMPILER_MIDEND_REMOVEPHI_H

#include "../llvm-ir/Module.h"
/*
 * <SSA Book> Algorithm 3.5
 */

class RemovePhi {

public:
	void run(Module *module);
	void parallelCopy(Function *f);
	void sequentialMove(Function *f);
};

#endif //COMPILER_MIDEND_REMOVEPHI_H
