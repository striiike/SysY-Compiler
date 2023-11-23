//
// Created by hiccup on 2023/11/14.
//

#pragma once

#include <string>
#include <utility>
using namespace std;

struct MipsOperand {
	virtual string toString() { return "!fuck!"; }
};

struct MipsReg : MipsOperand {
};

struct MipsVrReg : MipsReg {
	static int cnt;
	std::string name;

	MipsVrReg() {
		name = "Vr_" + to_string(++cnt);
	}

	string toString() override { return name; }
};

struct MipsPhReg : MipsReg {
	std::string name;

	explicit MipsPhReg(std::string name) : name(std::move(name)) {}
	string toString() override { return name; }
};

typedef MipsPhReg *MipsPhRegPtr;
extern MipsPhRegPtr $zero, $v0, $v1, $a0, $a1, $a2, $a3, $t0, $t1, t2, t3, t4, t5, t6, t7,
	s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, $sp, $fp, $ra;

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