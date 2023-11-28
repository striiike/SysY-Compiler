//
// Created by hiccup on 2023/11/28.
//

#include "DeadCodeEmit.h"
#include "../llvm-ir/Module.h"

/*
 *  temporarily not considering @call inst emitting
 */

void DeadCodeEmit::run(Module *module) {
	for (auto f : module->functionList) {

		usefulInsts.clear();

		/*
		 *  find closure for
		 *  	store, call, branch, return
		 *
		 *  if not in the closure, which means it's can be killed
		 */

		for (auto bb : f->basicList) {
			for (auto i : bb->instructionList) {

				if (i->instType==InstType::BRANCH ||
					i->instType==InstType::STORE ||
					i->instType==InstType::RETURN ||
					i->instType==InstType::CALL) {

					instClosure(i);
				}
			}
		}

		for (auto bb : f->basicList) {

			auto *instList = &(bb->instructionList);
			for (auto it = instList->begin();
				 it!=instList->end();) {

				if (!usefulInsts.count(*it)) {
					(*it)->removeUse();
					it = instList->erase(it);
				} else {
					++it;
				}

			}
		}
	}
}

void DeadCodeEmit::instClosure(Instruction *i) {
	if (usefulInsts.count(i))
		return;

	usefulInsts.insert(i);

	/*
	 *  if useful instruction needs it to calculate,
	 *  	then the operand is also useful.
	 */
	for (auto used : i->opList) {
		if (dynamic_cast<Instruction *>(used))
			instClosure((Instruction *)used);
	}
}

