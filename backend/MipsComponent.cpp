//
// Created by hiccup on 2023/11/14.
//

#include "MipsComponent.h"

string MipsGlobalVar::toString() const {
	if (arr.empty()) {
		return name + ": .asciiz \"" + stringLiteral + "\"";
	} else {
		stringstream ss;
		ss << name + ": .word ";
//			for (int i = (int)arr.size() - 1; i >= 0; --i) {
		for (int i : arr) {
			ss << to_string(i);
			ss << ", ";
		}
		return ss.str();
	}

}
