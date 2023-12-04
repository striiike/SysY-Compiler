//
// Created by hiccup on 2023/10/20.
//

#include <algorithm>
#include "User.h"

void User::updateOld2New(Value *old, Value *_new) {
	if (std::count(opList.begin(), opList.end(), old)) {
		std::replace(opList.begin(), opList.end(), old, _new);

		// TODO: delete old and update new
		old->useList.remove_if([this](Use *u) { return (u->user)==this; });
		(_new->useList).push_back(new Use(this, _new));
	}
}
Value *User::getOp(int index) {
	if (index >= opList.size()) return nullptr;
	return opList[index];
}
void User::addOp(Value *value) {
	assert(value!=nullptr);
	opList.push_back(value);
	value->useList.push_back(new Use(this, value));
}
User::User(Type *type, std::string name)
	: Value(type, std::move(name)) {
	opList.clear();
}

void User::removeSelf() {
	for (auto op : opList) {
		op->useList.remove_if([this](Use *u) { return (u->user)==this; });
	}
}