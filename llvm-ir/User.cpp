//
// Created by hiccup on 2023/10/20.
//

#include "User.h"
void User::updateOld2New(Value *old, Value *_new) {
	if (std::count(operandList.begin(), operandList.end(), old)) {
		std::replace(operandList.begin(), operandList.end(), old, _new);

		// TODO: delete old and update new
		old->useList.remove_if([this](Use *u) { return (u->user)==this; });
		(_new->useList).push_back(new Use(this, _new));
	}
}
