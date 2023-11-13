//
// Created by hiccup on 2023/10/20.
//

#include "IrBuilder.h"

IrBuilder irBuilder{};

IntegerType *IntegerType::VOID = new IntegerType(0);
IntegerType *IntegerType::INT1 = new IntegerType(1);
IntegerType *IntegerType::INT8 = new IntegerType(8);
IntegerType *IntegerType::INT32 = new IntegerType(32);

Function *Function::getint = new Function(IntegerType::INT32, "@getint");
Function *Function::putch = new Function(IntegerType::VOID, "@putch");
Function *Function::putint = new Function(IntegerType::VOID, "@putint");
Function *Function::putstr = new Function(IntegerType::VOID, "@putstr");

void IrBuilder::addFunction(Function *func) {
	module->addFunction(func);
}
void IrBuilder::addBasicBlock(BasicBlock *bb) {
	curFunc->addBasicBlock(bb);
	bb->setParent(curFunc);
}
void IrBuilder::addGlobalVariable(GlobalVariable *globalVar) {
	module->addGlobalVariable(globalVar);
}
std::string IrBuilder::genFuncName(const string &name) {
	if (name=="main") {
		return "@main";
	}
	return funcPrefix + name;
}
std::string IrBuilder::genBbName() {
	return bbPrefix + "_" + to_string(bbCnt++);
}
std::string IrBuilder::genGlobalVarName(const string &name) {
	return globalPrefix + name + "_" + to_string((globalVarCnt++));
}
std::string IrBuilder::genLocalVarName(const string &name) {
	return localVarPrefix + name + "_" + to_string((localVarCnt++));
//		return localVarPrefix + to_string((localVarCnt++));
}
std::string IrBuilder::genArgName(const string &name) {
	return argPrefix + name + "_" + to_string((argsCnt++));
}
BasicBlock *IrBuilder::buildBb() {
	string name = genBbName();
	auto *bb = new BasicBlock(name);
	return bb;
}
Value *IrBuilder::buildGlobalVar(const string &ident, Constant *constant, bool isConst) {
	string name = genGlobalVarName(ident);
	auto *globalVar = new GlobalVariable(new PointerType(constant->getType()), name, isConst, constant);
	module->addGlobalVariable(globalVar);
	return globalVar;
}
Value *IrBuilder::buildArgument(const string &ident, Type *ty) {
	string name = genArgName(ident);
	auto *arg = new Argument(ty, name);
	curFunc->addArgument(arg);
	return arg;
}
Value *IrBuilder::buildAlloc(const string &ident, Type *ty) {
	string name = genLocalVarName(ident);
	auto *alloca = new AllocaInst(ty, name);
	curBb->addInstruction(alloca, true);
	return alloca;
}
void IrBuilder::buildStore(Value *content, Value *addr) {
	auto *store = new StoreInst(content, addr);
	curBb->addInstruction(store);
}
Value *IrBuilder::buildLoad(Value *addr) {
	string name = genLocalVarName();
	auto *load = new LoadInst(name, addr);
	curBb->addInstruction(load);
	return load;
}
Value *IrBuilder::buildGEP(Value *base, Value *ptrOff, Value *arrOff) {
	string name = genLocalVarName();
	Instruction *gep;
	if (base->getType()->getTargetType()->isArray())
		gep = new GEPInst(name, base, new ConstantInt(0), ptrOff);
	else
		gep = new GEPInst(name, base, ptrOff, arrOff);

	curBb->addInstruction(gep);
	return gep;
}
Value *IrBuilder::buildAlu(AluType ty, Value *op1, Value *op2) {
	string name = genLocalVarName();
	auto *alu = new AluInst(name, ty, op1, op2);
	curBb->addInstruction(alu);
	return alu;
}
Value *IrBuilder::buildReturn(Value *ret) {
	auto *retInst = new ReturnInst("returnInst", ret);
	curBb->addInstruction(retInst);
	return retInst;
}
Value *IrBuilder::buildCall(Function *func, vector<Value *> args) {
	string name = genLocalVarName();
	auto *call = new CallInst(name, func, args);
	curBb->addInstruction(call);
	return call;
}
Value *IrBuilder::buildZext(Value *val, Type *tar) {
	string name = genLocalVarName();
	auto *zext = new ZextInst(name, val, tar);
	curBb->addInstruction(zext);
	return zext;
}
Value *IrBuilder::buildIcmpInst(IcmpType ty, Value *op1, Value *op2) {
	string name = genLocalVarName();
	auto *cmp = new IcmpInst(name, ty, op1, op2);
	curBb->addInstruction(cmp);
	return cmp;
}
Value *IrBuilder::buildBrInst(Value *cond, BasicBlock *bb1, BasicBlock *bb2) {
	auto *br = new BrInst(cond, bb1, bb2);
	curBb->addInstruction(br);
	return nullptr;
}
Value *IrBuilder::buildBrInst(BasicBlock *bb) {
	auto *br = new BrInst(bb);
	curBb->addInstruction(br);
	return nullptr;
}
Value *IrBuilder::buildPhiInst(vector<BasicBlock *> *bbs, BasicBlock *to) {
	string name = genLocalVarName();
	auto *phi = new PhiInst(bbs, name);
	to->addInstruction(phi, true);
	return phi;
}
void IrBuilder::fillInReturn() {
	if (!curBb->lastReturn()) {
		Value *val;
		if (ctx.voidFunc)
			val = new Value(IntegerType::VOID, "");
		else
			val = new ConstantInt(0);
		buildReturn(val);
	}
}
