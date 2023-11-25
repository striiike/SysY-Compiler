//
// Created by hiccup on 2023/11/14.
//

#pragma once

#include "MipsOperand.h"
#include <string>
#include <utility>
#include <unordered_set>
#include <unordered_set>

using namespace std;

struct MipsInst {

	virtual unordered_set<MipsReg *> *getDef() { return new unordered_set<MipsReg *>{}; }
	virtual unordered_set<MipsReg *> *getUse() { return new unordered_set<MipsReg *>{}; }
	virtual bool replaceUse2vr(MipsReg *old, MipsReg *_new) { return false; }
	virtual bool replaceDef2vr(MipsReg *old, MipsReg *_new) { return false; }
	virtual string toString() { return "!fuck!"; }
};

enum BinType {
	M_ADDU, M_SUBU, M_MUL, M_DIV, M_SREM, M_SLL
};

struct MipsBinInst : MipsInst {
	BinType ty;
	MipsOperand *src1;
	MipsOperand *src2;
	MipsOperand *dst;

	MipsBinInst(BinType t, MipsOperand *d, MipsOperand *s1, MipsOperand *s2)
		: ty(t), dst(d), src1(s1), src2(s2) {}

	string toString() override;

	unordered_set<MipsReg *> *getUse() override {
		auto s = new unordered_set<MipsReg *>;
		(dynamic_cast<MipsVrReg *>(src1) && s->insert((MipsVrReg *)src1).second);
		(dynamic_cast<MipsVrReg *>(src2) && s->insert((MipsVrReg *)src2).second);
		return s;
	}
	unordered_set<MipsReg *> *getDef() override {
		auto s = new unordered_set<MipsReg *>;
		(dynamic_cast<MipsVrReg *>(dst) && s->insert((MipsVrReg *)dst).second);
		return s;
	}
	bool replaceUse2vr(MipsReg *old, MipsReg *_new) override {
		return src1==old && (src1 = _new, true) ||
			src2==old && (src2 = _new, true);
	}
	bool replaceDef2vr(MipsReg *old, MipsReg *_new) override {
		return dst==old && (dst = _new, true);
	}
};

struct MipsLiInst : MipsInst {
	MipsOperand *dst;
	MipsOperand *imm;

	MipsLiInst(MipsOperand *dst, MipsOperand *imm) : dst(dst), imm(imm) {}

	string toString() override;

	unordered_set<MipsReg *> *getUse() override {
		auto s = new unordered_set<MipsReg *>;
		(dynamic_cast<MipsVrReg *>(imm) && s->insert((MipsVrReg *)imm).second);
		return s;
	}
	unordered_set<MipsReg *> *getDef() override {
		auto s = new unordered_set<MipsReg *>;
		(dynamic_cast<MipsVrReg *>(dst) && s->insert((MipsVrReg *)dst).second);
		return s;
	}
	bool replaceUse2vr(MipsReg *old, MipsReg *_new) override {
		return imm==old && (imm = _new, true);
	}
	bool replaceDef2vr(MipsReg *old, MipsReg *_new) override {
		return dst==old && (dst = _new, true);
	}
};

struct MipsMarco : MipsInst {
	string dst;

	explicit MipsMarco(string dst) : dst(std::move(dst)) {}

	string toString() override;
};

enum CondType {
	BEQ, BNE, BGT, BGE, BLT, BLE, BGEZ, BLEZ, BGTZ, BLTZ, J, JAL, JR
};

struct MipsBranchInst : MipsInst {
	CondType ty;
	MipsLabel *label;
	MipsOperand *src1{};
	MipsOperand *src2{};

	MipsBranchInst(CondType ty, MipsOperand *src1, MipsOperand *src2, MipsLabel *label)
		: ty(ty), src1(src1), src2(src2), label(label) {}

	MipsBranchInst(CondType ty, MipsLabel *label)
		: ty(ty), label(label) {}

	string toString() override;

	unordered_set<MipsReg *> *getUse() override {
		auto s = new unordered_set<MipsReg *>;
		(dynamic_cast<MipsVrReg *>(src1) && s->insert((MipsVrReg *)src1).second);
		(dynamic_cast<MipsVrReg *>(src2) && s->insert((MipsVrReg *)src2).second);
		return s;
	}
	bool replaceUse2vr(MipsReg *old, MipsReg *_new) override {
		return src1==old && (src1 = _new, true) ||
			src2==old && (src2 = _new, true);
	}
};

struct MipsCmpInst : MipsInst {
	CondType ty;
	MipsOperand *src1;
	MipsOperand *src2;
	MipsOperand *dst;

	MipsCmpInst(CondType t, MipsOperand *d, MipsOperand *s1, MipsOperand *s2)
		: ty(t), dst(d), src1(s1), src2(s2) {}

	string toString() override;

	unordered_set<MipsReg *> *getUse() override {
		auto s = new unordered_set<MipsReg *>;
		(dynamic_cast<MipsVrReg *>(src1) && s->insert((MipsVrReg *)src1).second);
		(dynamic_cast<MipsVrReg *>(src2) && s->insert((MipsVrReg *)src2).second);
		return s;
	}
	unordered_set<MipsReg *> *getDef() override {
		auto s = new unordered_set<MipsReg *>;
		(dynamic_cast<MipsVrReg *>(dst) && s->insert((MipsVrReg *)dst).second);
		return s;
	}
	bool replaceUse2vr(MipsReg *old, MipsReg *_new) override {
		return src1==old && (src1 = _new, true) ||
			src2==old && (src2 = _new, true);
	}
	bool replaceDef2vr(MipsReg *old, MipsReg *_new) override {
		return dst==old && (dst = _new, true);
	}
};

struct MipsStoreInst : MipsInst {
	MipsOperand *addr;
	MipsOperand *offset;
	MipsOperand *dst;

	MipsStoreInst(MipsOperand *dst, MipsOperand *addr, MipsOperand *off) :
		addr(addr), offset(off), dst(dst) {
	}

	string toString() override;

	unordered_set<MipsReg *> *getUse() override {
		auto s = new unordered_set<MipsReg *>;
		(dynamic_cast<MipsVrReg *>(offset) && s->insert((MipsVrReg *)offset).second);
		(dynamic_cast<MipsVrReg *>(addr) && s->insert((MipsVrReg *)addr).second);
		return s;
	}
	unordered_set<MipsReg *> *getDef() override {
		auto s = new unordered_set<MipsReg *>;
		(dynamic_cast<MipsVrReg *>(dst) && s->insert((MipsVrReg *)dst).second);
		return s;
	}
	bool replaceUse2vr(MipsReg *old, MipsReg *_new) override {
		return offset==old && (offset = _new, true) ||
			addr==old && (addr = _new, true);
	}
	bool replaceDef2vr(MipsReg *old, MipsReg *_new) override {
		return dst==old && (dst = _new, true);
	}

};

struct MipsLoadInst : MipsInst {
	MipsOperand *addr;
	MipsOperand *offset;
	MipsOperand *dst;

	MipsLoadInst(MipsOperand *dst, MipsOperand *addr, MipsOperand *off) :
		addr(addr), offset(off), dst(dst) {
	}

	string toString() override;

	unordered_set<MipsReg *> *getUse() override {
		auto s = new unordered_set<MipsReg *>;
		(dynamic_cast<MipsVrReg *>(offset) && s->insert((MipsVrReg *)offset).second);
		(dynamic_cast<MipsVrReg *>(addr) && s->insert((MipsVrReg *)addr).second);
		return s;
	}
	unordered_set<MipsReg *> *getDef() override {
		auto s = new unordered_set<MipsReg *>;
		(dynamic_cast<MipsVrReg *>(dst) && s->insert((MipsVrReg *)dst).second);
		return s;
	}
	bool replaceUse2vr(MipsReg *old, MipsReg *_new) override {
		return offset==old && (offset = _new, true) ||
			addr==old && (addr = _new, true);
	}
	bool replaceDef2vr(MipsReg *old, MipsReg *_new) override {
		return dst==old && (dst = _new, true);
	}
};

struct MipsComment : MipsInst {
	string comment;

	explicit MipsComment(string name) { comment = std::move(name); }

	string toString() override;
};



