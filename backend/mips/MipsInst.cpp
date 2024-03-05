//
// Created by hiccup on 2023/11/14.
//

#include "MipsInst.h"
std::string MipsBinInst::toString()  {
	if (ty==M_DIV) {
		return "div \t" + dst->toString() + ", \t " + src1->toString() + ", \t" + src2->toString();
	}
	if (ty==M_SREM) {
		return "rem \t" + dst->toString() + ", \t " + src1->toString() + ", \t" + src2->toString();
	}
	if (ty==M_ADDU) {
		if (dynamic_cast<MipsImm*>(src2)) {
			return "addiu \t" + dst->toString() + ", \t " + src1->toString() + ", \t" + src2->toString();
		}
		return "addu \t" + dst->toString() + ", \t " + src1->toString() + ", \t" + src2->toString();
	}
	if (ty==M_SUBU) {
		if (dynamic_cast<MipsImm*>(src2)) {
			return "addiu \t" + dst->toString() + ", \t " + src1->toString() + ", \t-" + src2->toString();
		}
		return "subu \t" + dst->toString() + ", \t " + src1->toString() + ", \t" + src2->toString();
	}
	if (ty==M_MUL) {
		return "mul \t" + dst->toString() + ", \t " + src1->toString() + ", \t" + src2->toString();
	}
	if (ty==M_SLL) {
		return "sll \t" + dst->toString() + ", \t " + src1->toString() + ", \t" + src2->toString();
	}
	if (ty==M_SRL) {
		return "srl \t" + dst->toString() + ", \t " + src1->toString() + ", \t" + src2->toString();
	}
	if (ty==M_SRA) {
		return "sra \t" + dst->toString() + ", \t " + src1->toString() + ", \t" + src2->toString();
	}
	return "!fuck!";
}
std::string MipsLiInst::toString()  {
	if (dynamic_cast<MipsLabel *>(imm)) {
		return "la   \t" + dst->toString() + ", \t" + imm->toString();
	} else if (dynamic_cast<MipsReg *>(imm)) {
		return "move \t" + dst->toString() + ", \t" + imm->toString();
	}
	return "li   \t" + dst->toString() + ", \t" + imm->toString();
}
std::string MipsMarco::toString()  {

	return dst;
}

std::string MipsBranchInst::toString()  {
	if (src2) {
		if (ty==BNE) {
			return "bne \t" + src1->toString() + ", \t" + src2->toString() + ", \t" + label->toString();
		}
	} else {
		if (ty==J) {
			return "j    \t" + label->toString();
		}
		if (ty==JAL) {
			return "jal  \t" + label->toString();
		}
		if (ty==JR) {
			return "jr   \t" + src1->toString();
		}
	}
	return "";
}

std::string MipsCmpInst::toString()  {
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
std::string MipsStoreInst::toString()  {
	return "sw   \t" + dst->toString() + ", \t" + addr->toString() + "(" + offset->toString() + ")";
}
std::string MipsLoadInst::toString()  {
	return "lw   \t" + dst->toString() + ", \t" + addr->toString() + "(" + offset->toString() + ")";
}
std::string MipsComment::toString()  {
	return "# --- " + comment + " --- #";
}
