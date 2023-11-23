//
// Created by hiccup on 2023/11/14.
//

#include "MipsOperand.h"

int MipsVrReg::cnt = 0;

MipsPhRegPtr $zero = new MipsPhReg("$zero");
MipsPhRegPtr $v0 = new MipsPhReg("$v0");
MipsPhRegPtr $v1 = new MipsPhReg("$v1");
MipsPhRegPtr $a0 = new MipsPhReg("$a0");
MipsPhRegPtr $a1 = new MipsPhReg("$a1");
MipsPhRegPtr $a2 = new MipsPhReg("$a2");
MipsPhRegPtr $a3 = new MipsPhReg("$a3");
MipsPhRegPtr $t0 = new MipsPhReg("$t0");
MipsPhRegPtr $t1 = new MipsPhReg("$t1");
MipsPhRegPtr $t2 = new MipsPhReg("$t2");
MipsPhRegPtr $t3 = new MipsPhReg("$t3");
MipsPhRegPtr $t4 = new MipsPhReg("$t4");
MipsPhRegPtr $t5 = new MipsPhReg("$t5");
MipsPhRegPtr $t6 = new MipsPhReg("$t6");
MipsPhRegPtr $t7 = new MipsPhReg("$t7");
MipsPhRegPtr $s0 = new MipsPhReg("$s0");
MipsPhRegPtr $s1 = new MipsPhReg("$s1");
MipsPhRegPtr $s2 = new MipsPhReg("$s2");
MipsPhRegPtr $s3 = new MipsPhReg("$s3");
MipsPhRegPtr $s4 = new MipsPhReg("$s4");
MipsPhRegPtr $s5 = new MipsPhReg("$s5");
MipsPhRegPtr $s6 = new MipsPhReg("$s6");
MipsPhRegPtr $s7 = new MipsPhReg("$s7");
MipsPhRegPtr $t8 = new MipsPhReg("$t8");
MipsPhRegPtr $t9 = new MipsPhReg("$t9");
MipsPhRegPtr $k0 = new MipsPhReg("$k0");
MipsPhRegPtr $k1 = new MipsPhReg("$k1");
MipsPhRegPtr $gp = new MipsPhReg("$gp");
MipsPhRegPtr $sp = new MipsPhReg("$sp");
MipsPhRegPtr $fp = new MipsPhReg("$fp");
MipsPhRegPtr $ra = new MipsPhReg("$ra");