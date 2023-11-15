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
	M_ADDU, M_SUBU, M_MUL, M_DIV, M_SREM
};

struct MipsBinInst : MipsInst {
	BinType ty;
	MipsOperand *src1;
	MipsOperand *src2;
	MipsOperand *dst;

	std::string toString() const override {
		if (ty==M_DIV) {
			return "div \t" + dst->toString() + ", \t " + src1->toString() + ", \t" + src2->toString();
		}
		if (ty==M_SREM) {
			return "rem \t" + dst->toString() + ", \t " + src1->toString() + ", \t" + src2->toString();
		}
		if (ty==M_ADDU) {
			return "addu \t" + dst->toString() + ", \t " + src1->toString() + ", \t" + src2->toString();
		}
		if (ty==M_SUBU) {
			return "subu \t" + dst->toString() + ", \t " + src1->toString() + ", \t" + src2->toString();
		}
		if (ty==M_MUL) {
			return "mul \t" + dst->toString() + ", \t " + src1->toString() + ", \t" + src2->toString();
		}
		return "!fuck!";
	};
};

struct MipsLiInst : MipsInst {
	MipsOperand *dst;
	MipsOperand *imm;

	MipsLiInst(MipsOperand *dst, MipsOperand *imm) : dst(dst), imm(imm) {

	}
	std::string toString() const override {
		return "li \t" + dst->toString() + ", \t" + imm->toString();
	}
};

struct MipsMoveInst : MipsInst {
	MipsOperand *src1;
	MipsOperand *dst;
	std::string toString() const override {
		return "move \t" + dst->toString() + ", \t" + src1->toString();
	}
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

	std::string toString() const override {
		if (src2) {
			if (ty==BNE) {
				return "bne\t" + src1->toString() + ", \t" + src2->toString() + ", \t" + label->toString();
			}
		} else {
			if (ty==J) {
				return "j\t" + label->toString();
			}
			if (ty==JAL) {
				return "jal\t" + label->toString();
			}
			if (ty==JR) {
				return "jr\t" + src1->toString();
			}
		}
	}

};

struct MipsCmpInst : MipsInst {
	CondType ty;
	MipsOperand *src1;
	MipsOperand *src2;
	MipsOperand *dst;

	std::string toString() const override {
		switch (ty) {
		case BEQ:return "seq \t" + dst->toString() + ", \t" + src1->toString() + ", \t" + src2->toString();
		case BNE:return "sne \t" + dst->toString() + ", \t" + src1->toString() + ", \t" + src2->toString();
		case BLE:return "sle \t" + dst->toString() + ", \t" + src1->toString() + ", \t" + src2->toString();
		case BLT:return "slt \t" + dst->toString() + ", \t" + src1->toString() + ", \t" + src2->toString();
		case BGE:return "sge \t" + dst->toString() + ", \t" + src1->toString() + ", \t" + src2->toString();
		case BGT:return "sgt \t" + dst->toString() + ", \t" + src1->toString() + ", \t" + src2->toString();

		default:return "!fuck!";
		}


	}
};

struct MipsStoreInst : MipsInst {
	MipsOperand *addr;
	MipsOperand *offset;
	MipsOperand *dst;

	MipsStoreInst(MipsOperand *dst, MipsOperand *addr, MipsOperand *off) :
		addr(addr), offset(off), dst(dst) {
	}

	std::string toString() const override {
		return "sw \t" + dst->toString() + ", \t" + addr->toString() + "(" + offset->toString() + ")";
	}
};

struct MipsLoadInst : MipsInst {
	MipsOperand *addr;
	MipsOperand *offset;
	MipsOperand *dst;

	MipsLoadInst(MipsOperand *dst, MipsOperand *addr, MipsOperand *off) :
		addr(addr), offset(off), dst(dst) {
	}

	std::string toString() const override {
		return "lw \t" + dst->toString() + ", \t" + addr->toString() + "(" + offset->toString() + ")";
	}
};

struct MipsComment : MipsInst {
	std::string comment;

	MipsComment(std::string name) { comment = std::move(name); }

	std::string toString() const override {
		return "# --- " + comment + " --- #";
	}
};



