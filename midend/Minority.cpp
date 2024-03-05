//
// Created by hiccup on 2023/12/10.
//

#include "Minority.h"
#include "../llvm-ir/Module.h"

void Minority::run(Module *module) {
	for (auto f: module->functionList) {
		killZext(f);
	}
}

void Minority::killZext(Function * f) {
	for (auto bb : f->basicList){
		auto instList = &bb->instructionList;
		for (auto it = instList->begin(); it != instList->end();) {
			if ((*it)->instType == InstType::ZEXT) {
				(*it)->replaceOld2New((*it)->getOp(0));
				it = instList->erase(it);
			} else {
				++it;
			}
		}
	}
 }