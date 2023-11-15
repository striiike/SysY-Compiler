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
	std::string name;

	explicit MipsReg(std::string name) : name(std::move(name)) {}
	string toString() override { return name; }
};

typedef MipsReg *MipsRegPtr;
extern MipsRegPtr $zero, v0, v1, a0, a1, a2, a3, $t0, $t1, t2, t3, t4, t5, t6, t7,
	s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, $sp, fp, ra;

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