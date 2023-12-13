//
// Created by hiccup on 2023/11/30.
//

#include "PeepHole.h"
#include "mips/MipsComponent.h"

void PeepHole::run(MipsModule *module) {
	bool flag = true;

	for (auto f : module->functions) {
		for (auto bb : f->blockList) {

			auto instList = &(bb->instList);
			for (auto it = instList->begin(); it!=instList->end();) {

				auto inst = *it;
				if (dynamic_cast<MipsBinInst *>(inst)) {
					auto *_inst = dynamic_cast<MipsBinInst *>(inst);
					if (_inst->ty==BinType::M_ADDU || _inst->ty==BinType::M_SUBU) {

						if (((MipsReg *)_inst->dst)->reg()==((MipsReg *)_inst->src1)->reg()) {

							if (dynamic_cast<MipsReg *>(_inst->src2) && ((MipsReg *)_inst->src2)->reg()==$zero) {
								it = instList->erase(it);
								continue;
							} else if (dynamic_cast<MipsImm *>(_inst->src2) && ((MipsImm *)_inst->src2)->imm==0) {
								it = instList->erase(it);
								continue;
							}

						}

					}
				}
				++it;
			}

		}
	}


	for (auto f : module->functions) {
		for (auto it = f->blockList.begin(); it != f->blockList.end(); ++it) {

			++it;
			auto *nit = &(*it);
			auto nbb = (*nit);
			--it;

			if (*nit) {
				auto last = (*it)->lastInst();
				auto br = dynamic_cast<MipsBranchInst *>(last);

				if (br && br->ty == J && br->label->label == (*nit)->label) {
					(*it)->instList.pop_back();
				}


			}



		}
	}

}

bool PeepHole::imm2Zero(_List_iterator<MipsInst *> it, MipsBlock *bb) {

	bool flag = true;

}

// 烂尾 典