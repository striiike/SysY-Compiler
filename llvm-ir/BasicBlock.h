//
// Created by hiccup on 2023/10/20.
//

#ifndef COMPILER_LLVM_IR_BASICBLOCK_H
#define COMPILER_LLVM_IR_BASICBLOCK_H

#include <list>
#include "instruction/Instruction.h"
#include <sstream>
#include "Value.h"

using namespace std;

class Function;

class BasicBlock : public Value {
public:
	std::list<Instruction *> instructionList;
	Function *parent{};

	// CFG
	vector<BasicBlock *> *preBbs{};
	vector<BasicBlock *> *sucBbs{};

	/// dominateTree
	BasicBlock *immDominator = nullptr;
	vector<BasicBlock *> *childBbs{};

	/// DF
	vector<BasicBlock *> *DF{};

public:
	explicit BasicBlock(std::string name)
		: Value(nullptr, std::move(name)) {}

	void setParent(Function *func);

	void addInstruction(Instruction *inst, bool head = false);

	bool lastReturn();

	std::string toString() override;

	Instruction *getEndInst();
};

#endif //COMPILER_LLVM_IR_BASICBLOCK_H
