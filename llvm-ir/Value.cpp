//
// Created by hiccup on 2023/10/20.
//

#include "Value.h"
#include "User.h"

void Value::replaceOld2New(Value *_new) {
	std::vector<User*> userList{};
	for (auto use : useList) userList.push_back(use->user);

	for (auto user : userList) {
		user->updateOld2New(this, _new);
	}
}
