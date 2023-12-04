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

	int dtDepth = 0;

public:
	explicit BasicBlock(std::string name)
		: Value(nullptr, std::move(name)) {}

	void setParent(Function *func) {
		parent = func;
	}

	void addInstruction(Instruction *inst, bool head = false) {
		if (head)
			instructionList.push_front(inst);
		else
			instructionList.push_back(inst);
		inst->parent = this;
	}

	bool lastReturn() {
		if (instructionList.empty())
			return false;
		return instructionList.back()->instType==InstType::RETURN;
	}

	std::string toString() override {
		std::stringstream ss;
		ss << name + ":\n";
		for (auto i : instructionList) {
			ss << "\t";
			ss << i->toString();
			ss << "\n";
		}
		return ss.str();
	}

	Instruction *getEndInst() {
		return instructionList.back();
	}
};

#endif //COMPILER_LLVM_IR_BASICBLOCK_H
