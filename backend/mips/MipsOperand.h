//
// Created by hiccup on 2023/11/14.
//

#pragma once

#include <string>
#include <utility>
#include <map>
using namespace std;

struct MipsPhReg;

struct MipsOperand {
	virtual string toString() { return "!fuck!"; }
};

struct MipsReg : MipsOperand {

	virtual MipsPhReg *reg() { return nullptr; }
	virtual bool isPreColored() { return false; }
};

struct MipsPhReg : MipsReg {
	std::string name;

	explicit MipsPhReg(std::string name) : name(std::move(name)) {}
	MipsPhReg *reg() override { return this; }
	string toString() override { return name; }
	bool isPreColored() override { return true; }
};

struct MipsVrReg : MipsReg {
	static int cnt;
	std::string name;

	MipsPhReg *alloca = nullptr;

	MipsVrReg() {
		name = "%VR" + to_string(++cnt);
	}

	string toString() override {
		if (alloca)
			return alloca->name;
		return name;
	}
	MipsPhReg *reg() override { return alloca; }
};

typedef MipsPhReg *MipsPhRegPtr;
extern MipsPhRegPtr $zero, $v0, $v1, $a0, $a1, $a2, $a3, $t0, $t1, t2, t3, t4, t5, t6, t7,
	s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, $sp, $fp, $ra;
extern map<int, MipsPhRegPtr> index2reg;

struct MipsImm : MipsOperand {
	int imm;

	explicit MipsImm(int i) : imm(i) {}
	string toString() override { return to_string(imm); }
};

struct MipsLabel : MipsOperand {
	string label;

	explicit MipsLabel(string l) : label(std::move(l)) {}
	string toString() override { return label; }
};