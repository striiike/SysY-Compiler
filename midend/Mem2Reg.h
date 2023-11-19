//
// Created by hiccup on 2023/11/9.
//

#ifndef COMPILER_MIDEND_MEM2REG_H
#define COMPILER_MIDEND_MEM2REG_H

#include <set>
#include <stack>
using namespace std;

class BasicBlock;
class Instruction;
class Value;
class Module;

class Mem2Reg {
public:
	set<BasicBlock *> *defBbs = new std::set<BasicBlock *>();
	set<BasicBlock *> *useBbs = new std::set<BasicBlock *>();
	set<Instruction *> *defInsts = new std::set<Instruction *>();
	set<Instruction *> *useInsts = new std::set<Instruction *>();

	stack<Value *> *reach = new stack<Value *>();
public:
	Mem2Reg() {

	}
	void run(Module *module);
	void buildDefUse(Instruction *inst);
	void insertPhiInst(Instruction *inst);
	void rename(BasicBlock *entry);
};

#endif //COMPILER_MIDEND_MEM2REG_H
