//
// Created by hiccup on 2023/11/14.
//

#pragma once

#include "MipsOperand.h"
#include <string>
#include <utility>

struct MipsInst {

	virtual std::string toString() const { return "!fuck!"; }
};

enum BinType {
	M_ADDU, M_SUBU, M_MUL, M_DIV, M_SREM, M_SLL
};

struct MipsBinInst : MipsInst {
	BinType ty;
	MipsOperand *src1;
	MipsOperand *src2;
	MipsOperand *dst;

	std::string toString() const override;;
};

struct MipsLiInst : MipsInst {
	MipsOperand *dst;
	MipsOperand *imm;

	MipsLiInst(MipsOperand *dst, MipsOperand *imm) : dst(dst), imm(imm) {

	}
	std::string toString() const override;
};

struct MipsMarco : MipsInst {
	string dst;

	MipsMarco(string dst) : dst(std::move(dst)) {

	}

	std::string toString() const override;
};

struct MipsMoveInst : MipsInst {
	MipsOperand *src1;
	MipsOperand *dst;
	std::string toString() const override;
};

enum CondType {
	BEQ, BNE, BGT, BGE, BLT, BLE, BGEZ, BLEZ, BGTZ, BLTZ, J, JAL, JR
};

struct MipsBranchInst : MipsInst {
	CondType ty;
	MipsLabel *label;
	MipsOperand *src1;
	MipsOperand *src2;

	MipsBranchInst(CondType ty, MipsOperand *src1, MipsOperand *src2, MipsLabel *label)
		: ty(ty), src1(src1), src2(src2), label(label) {}

	std::string toString() const override;

};

struct MipsCmpInst : MipsInst {
	CondType ty;
	MipsOperand *src1;
	MipsOperand *src2;
	MipsOperand *dst;

	std::string toString() const override;
};

struct MipsStoreInst : MipsInst {
	MipsOperand *addr;
	MipsOperand *offset;
	MipsOperand *dst;

	MipsStoreInst(MipsOperand *dst, MipsOperand *addr, MipsOperand *off) :
		addr(addr), offset(off), dst(dst) {
	}

	std::string toString() const override;
};

struct MipsLoadInst : MipsInst {
	MipsOperand *addr;
	MipsOperand *offset;
	MipsOperand *dst;

	MipsLoadInst(MipsOperand *dst, MipsOperand *addr, MipsOperand *off) :
		addr(addr), offset(off), dst(dst) {
	}

	std::string toString() const override;
};

struct MipsComment : MipsInst {
	std::string comment;

	MipsComment(std::string name) { comment = std::move(name); }

	std::string toString() const override;
};



