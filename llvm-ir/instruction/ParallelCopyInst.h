//
// Created by hiccup on 2023/11/22.
//

#pragma once

#include "Instruction.h"

// these two are not llvm instruction, they are for mips

class ParallelCopyInst : public Instruction {
public:
	vector<Value *> dst;
	vector<Value *> src;
public:
	ParallelCopyInst() : Instruction(IntegerType::VOID, "fuckerPc", InstType::PC) {}

	void copyPair(Value *left, Value *right) {
		dst.push_back(left);
		src.push_back(right);
	}

	vector<pair<Value *, Value *>> pairList() {
		vector<pair<Value *, Value *>> l{};
		l.reserve(dst.size());
		for (int i = 0; i < dst.size(); ++i) {
			l.emplace_back(dst[i], src[i]);
		}
		return l;
	}

	std::string toString() override {
		std::stringstream ss;
		ss << "pc : ";
		for (int i = 0; i < dst.size(); i++) {
			ss << dst[i]->name + ", " + src[i]->name + " ";
		}
		return ss.str();
	}

};

class MoveInst : public Instruction {
public:
	Value *dst{}, *src{};
public:
	MoveInst(Value *dst, Value *src)
		: dst(dst), src(src),
		  Instruction(IntegerType::VOID, "fuckerMove", InstType::MOVE) {}

	std::string toString() override {
		return "move " + dst->name + " " + src->name;
	}

};