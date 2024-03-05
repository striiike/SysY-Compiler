//
// Created by hiccup on 2023/12/10.
//

#ifndef COMPILER_MIDEND_MINORITY_H
#define COMPILER_MIDEND_MINORITY_H

#include "../llvm-ir/Function.h"
#include "../llvm-ir/Module.h"
class Minority {

	void killZext(Function *f);
public:
	void run(Module *module);
};

#endif //COMPILER_MIDEND_MINORITY_H
