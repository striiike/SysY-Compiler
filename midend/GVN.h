//
// Created by hiccup on 2023/11/30.
//

#ifndef COMPILER_MIDEND_GVN_H
#define COMPILER_MIDEND_GVN_H

#include <unordered_map>
#include <string>
#include "../llvm-ir/instruction/Instruction.h"
#include "../llvm-ir/Module.h"

using namespace std;

class GVN {
	unordered_map<string, Instruction*> gvnMap;
	unordered_map<string, int> gvnCnt;

public:

	GVN() = default;
	void gvn_search(BasicBlock* bb);
	void mergeInst(string hash, Instruction* inst);
	bool mergeIntoGvnMap(Instruction* inst);
	void removeInst(string hash, Instruction* i);
	void removeFromGvnMap(Instruction* inst);
	void scheduleEarlyInFunction(Function* f);
	void scheduleLateInFunction(Function* f);
	bool constFold(Instruction* inst);
	bool canGVN(Instruction* inst);
	void run(Module* module);
};

#endif //COMPILER_MIDEND_GVN_H
