//
// Created by hiccup on 2023/11/14.
//

#include "MipsInst.h"
std::string MipsBinInst::toString() const {
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
	if (ty==M_SLL) {
		return "sll \t" + dst->toString() + ", \t " + src1->toString() + ", \t" + src2->toString();
	}
	return "!fuck!";
}
std::string MipsLiInst::toString() const {
	if (dynamic_cast<MipsLabel *>(imm)) {
		return "la \t" + dst->toString() + ", \t" + imm->toString();
	}
	return "li \t" + dst->toString() + ", \t" + imm->toString();
}
std::string MipsMarco::toString() const {

	return dst;
}
std::string MipsMoveInst::toString() const {
	return "move \t" + dst->toString() + ", \t" + src1->toString();
}
std::string MipsBranchInst::toString() const {
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

std::string MipsCmpInst::toString() const {
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
std::string MipsStoreInst::toString() const {
	return "sw \t" + dst->toString() + ", \t" + addr->toString() + "(" + offset->toString() + ")";
}
std::string MipsLoadInst::toString() const {
	return "lw \t" + dst->toString() + ", \t" + addr->toString() + "(" + offset->toString() + ")";
}
std::string MipsComment::toString() const {
	return "# --- " + comment + " --- #";
}
