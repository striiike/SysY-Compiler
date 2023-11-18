//
// Created by hiccup on 2023/10/20.
//

#include "BasicBlock.h"
void BasicBlock::addInstruction(Instruction *inst, bool head) {
	if (head)
		instructionList.push_front(inst);
	else
		instructionList.push_back(inst);
	inst->parent = this;
}
bool BasicBlock::lastReturn() {
	if (instructionList.empty())
		return false;
	return instructionList.back()->instType==InstType::RETURN;
}
std::string BasicBlock::toString() {
	std::stringstream ss;
	ss << name + ":\n";
	for (auto i : instructionList) {
		ss << "\t";
		ss << i->toString();
		ss << "\n";
	}
	return ss.str();
}
Instruction *BasicBlock::getEndInst() {
	return instructionList.back();
}
void BasicBlock::setParent(Function *func) {
	parent = func;
}
