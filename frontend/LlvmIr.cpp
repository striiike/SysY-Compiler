//
// Created by hiccup on 2023/11/21.
//
#include "nodes-include/lib.h"
#include "parser/Symbol.h"
#include "../llvm-ir/IrBuilder.h"

Value *Block::llvmIr() {
	if (!irBuilder.ctx.afterFuncDef)
		symbol.startScope();
	else
		irBuilder.ctx.afterFuncDef = false;

	for (const auto &i : blockItemPtrs) {
		i->llvmIr();
	}

	symbol.endScope();
	return nullptr;
}

Value *BlockItem::llvmIr() {
	if (declPtr)
		declPtr->llvmIr();
	if (stmtPtr)
		stmtPtr->llvmIr();
	return nullptr;
}


Value *IfStmt::llvmIr() {
	BasicBlock *thenBb, *elseBb, *endBb;
	thenBb = irBuilder.buildBb();
	if (stmtElsePtr)
		elseBb = irBuilder.buildBb();
	endBb = irBuilder.buildBb();

	irBuilder.ctx.thenBb = thenBb;
	irBuilder.ctx.endBb = (stmtElsePtr) ? elseBb : endBb;

	condPtr->llvmIr();

	irBuilder.setCurBb(thenBb);
	irBuilder.addBasicBlock(thenBb);
	stmtPtr->llvmIr();
	irBuilder.buildBrInst(endBb);

	if (stmtElsePtr) {
		irBuilder.setCurBb(elseBb);
		irBuilder.addBasicBlock(elseBb);
		stmtElsePtr->llvmIr();
		irBuilder.buildBrInst(endBb);
	}

	irBuilder.setCurBb(endBb);
	irBuilder.addBasicBlock(endBb);

	irBuilder.ctx.thenBb = nullptr;
	irBuilder.ctx.endBb = nullptr;

	return nullptr;
}

Value *_ForStmt::llvmIr() {
	auto *rvalue = expPtr->llvmIr();
	auto *lval = lValPtr->llvmIrAddr();
	irBuilder.buildStore(rvalue, lval);
	/// no need for sysY
	return nullptr;
}

Value *ForStmt::llvmIr() {
	if (assignStmtPtr1)
		assignStmtPtr1->llvmIr();

	BasicBlock *condBb, *bodyBb, *endBb, *incBb;
	condBb = irBuilder.buildBb();
	irBuilder.buildBrInst(condBb);

	irBuilder.setCurBb(condBb);
	irBuilder.addBasicBlock(condBb);

	bodyBb = irBuilder.buildBb();
	incBb = irBuilder.buildBb();
	endBb = irBuilder.buildBb();

	irBuilder.incStack.push(incBb);
	irBuilder.endStack.push(endBb);
	irBuilder.ctx.condBb = condBb;
	irBuilder.ctx.thenBb = bodyBb;
	irBuilder.ctx.endBb = endBb;

	if (condPtr)
		condPtr->llvmIr();
	else
		irBuilder.buildBrInst(bodyBb);

	irBuilder.setCurBb(bodyBb);
	irBuilder.addBasicBlock(bodyBb);
	if (stmtPtr)
		stmtPtr->llvmIr();

	irBuilder.buildBrInst(incBb);
	irBuilder.setCurBb(incBb);
	irBuilder.addBasicBlock(incBb);

	if (assignStmtPtr2)
		assignStmtPtr2->llvmIr();
	irBuilder.buildBrInst(condBb);

	irBuilder.setCurBb(endBb);
	irBuilder.addBasicBlock(endBb);

	irBuilder.incStack.pop();
	irBuilder.endStack.pop();
	irBuilder.ctx.thenBb = nullptr;
	irBuilder.ctx.condBb = nullptr;
	irBuilder.ctx.endBb = nullptr;

	return nullptr;
}

Value *CompUnit::llvmIr() {
	irBuilder.setModule(new Module());
	symbol.startScope();
	irBuilder.ctx.isGlobal = true;
	for (const auto &i : declPtrs)
		i->llvmIr();
	for (const auto &i : funcDefPtrs)
		i->llvmIr();
	mainFuncDefPtr->llvmIr();
	symbol.endScope();
	return nullptr;
}

Value *Decl::llvmIr() {
	irBuilder.ctx.isConst = this->isConst;
	for (const auto &i : defPtrs)
		i->llvmIr();
	irBuilder.ctx.isConst = false;
	return nullptr;
}


void Def::buildGlobalDef(const vector<int> &lens, int length) {
	vector<int> initValue{};
	if (initValPtr)
		initValue = initValPtr->evaluate();

	Constant *constant;
	// single variable
	if (expPtrs.empty()) {
		constant = new ConstantInt(initValPtr ? initValue[0] : 0, 32);
	} else {
		constant = new ConstantArray(initValue, length);
	}

	auto *globalVar = irBuilder.buildGlobalVar(ident.getValue(), constant, isConst);
	symbol.insertVar(isConst, ident.getValue(), lens, initValue, globalVar);
}

void Def::buildLocalDef(const vector<int> &lens, int length) {
	vector<int> initValue{};
	Value *alloca = nullptr;

	if (isConst) {
		Constant *constant;
		if (expPtrs.empty()) {
			/// \SEEMS_NO \TODO
			/// sysY limits that initial can be evaluated.
			if (initValPtr) {
				initValue = initValPtr->evaluate();
				alloca = new ConstantInt(initValue[0]);
			}
		} else {
			alloca = irBuilder.buildAlloc(ident.getValue(), new ArrayType(length, IntegerType::INT32));
			if (initValPtr) {
				initValue = initValPtr->evaluate();
				for (int i = 0; i < length; ++i) {
					Value *gep = irBuilder.buildGEP(
						alloca,
						new ConstantInt(i)
					);
					irBuilder.buildStore(new ConstantInt(initValue[i]), gep);
				}
			}
		}

	} else {

		if (expPtrs.empty()) {
			alloca = irBuilder.buildAlloc(ident.getValue(), IntegerType::INT32);
			if (initValPtr) {
				Value *value = initValPtr->llvmIr();
				irBuilder.buildStore(value, alloca);
			}
		} else {
			/// \SEEMS_NO \TODO
			alloca = irBuilder.buildAlloc(ident.getValue(), new ArrayType(length, IntegerType::INT32));
			if (initValPtr) {
				vector<Value *> irList = initValPtr->llvmIrList();
				for (int i = 0; i < length; ++i) {
					Value *gep = irBuilder.buildGEP(
						alloca,
						new ConstantInt(i)
					);
					irBuilder.buildStore(irList[i], gep);
				}
			}
		}

	}
	symbol.insertVar(isConst, ident.getValue(), lens, initValue, alloca);
}

Value *Def::llvmIr() {

	/// grammar @ensures that there is no VLA
	vector<int> dims{};
	int length = 1;
	dims.reserve(expPtrs.size());
	for (auto &i : expPtrs) {
		int val = i->evaluate();
		dims.push_back(val);
		length *= val;
	}

	/// ----------------- building llvm ir starts here ----------------- ///
	if (irBuilder.ctx.isGlobal) {
		buildGlobalDef(dims, length);
	} else {
		buildLocalDef(dims, length);
	}

	return nullptr;
}

int Number::evaluate() {
	return num;
}

Value *Number::llvmIr() {
	return new ConstantInt(num);
}


Value *Exp::llvmIr() {
	return addExpPtr->llvmIr();
}

// when computing global variable
int Exp::evaluate() {
	return addExpPtr->evaluate();
}


Value *Cond::llvmIr() {
	return lOrExpPtr->llvmIr();
}

Value *MulExp::llvmIr() {
	auto *sum = leftOperand->llvmIr();
	for (int i = 0; i < operators.size(); ++i) {
		if (operators[i]==TokenType::MULT) {
			sum = irBuilder.buildAlu(AluType::MUL, sum, operands[i]->llvmIr());
		}
		if (operators[i]==TokenType::DIV) {
			sum = irBuilder.buildAlu(AluType::SDIV, sum, operands[i]->llvmIr());
		}
		if (operators[i]==TokenType::MOD) {
			sum = irBuilder.buildAlu(AluType::SREM, sum, operands[i]->llvmIr());
		}
	}
	return sum;
}

Value *AddExp::llvmIr() {
	auto *sum = leftOperand->llvmIr();
	for (int i = 0; i < operators.size(); ++i) {
		if (operators[i]==TokenType::PLUS) {
			sum = irBuilder.buildAlu(AluType::ADD, sum, operands[i]->llvmIr());
		}
		if (operators[i]==TokenType::MINU) {
			sum = irBuilder.buildAlu(AluType::SUB, sum, operands[i]->llvmIr());
		}
	}
	return sum;
}

Value *RelExp::llvmIr() {
	Value *val = leftOperand->llvmIr();
	for (int i = 0; i < operators.size(); ++i) {
		if (val->type->isInt1())
			val = irBuilder.buildZext(val, IntegerType::INT32);
		Value *cmpVal = operands[i]->llvmIr();
		if (cmpVal->type->isInt1())
			cmpVal = irBuilder.buildZext(cmpVal, IntegerType::INT32);
		if (operators[i]==TokenType::LSS) {
			val = irBuilder.buildIcmpInst(IcmpType::LT, val, cmpVal);
		}
		if (operators[i]==TokenType::LEQ) {
			val = irBuilder.buildIcmpInst(IcmpType::LE, val, cmpVal);
		}
		if (operators[i]==TokenType::GRE) {
			val = irBuilder.buildIcmpInst(IcmpType::GT, val, cmpVal);
		}
		if (operators[i]==TokenType::GEQ) {
			val = irBuilder.buildIcmpInst(IcmpType::GE, val, cmpVal);
		}
	}
	return val;
}

Value *LAndExp::llvmIrAnd(BasicBlock *failure) {
	if (operands.empty()) {
		auto *val = leftOperand->llvmIr();
		irBuilder.buildBrInst(val, irBuilder.ctx.thenBb, failure);
	} else {
		auto *val = leftOperand->llvmIr();
		BasicBlock *bb = irBuilder.buildBb();
		irBuilder.buildBrInst(val, bb, failure);
		irBuilder.setCurBb(bb);
		irBuilder.addBasicBlock(bb);

		int len = (int)operators.size();
		for (int i = 0; i < len; ++i) {
			if (i==len - 1) {
				val = operands[i]->llvmIr();
				irBuilder.buildBrInst(val, irBuilder.ctx.thenBb, failure);
			} else {
				val = operands[i]->llvmIr();
				bb = irBuilder.buildBb();
				irBuilder.buildBrInst(val, bb, failure);
				irBuilder.setCurBb(bb);
				irBuilder.addBasicBlock(bb);
			}
		}
	}

	return nullptr;
}

Value *LOrExp::llvmIr() {
	if (operands.empty()) {
		leftOperand->llvmIrAnd(irBuilder.ctx.endBb);

	} else {
		BasicBlock *bb = irBuilder.buildBb();
		leftOperand->llvmIrAnd(bb);
		irBuilder.setCurBb(bb);
		irBuilder.addBasicBlock(bb);

		int len = (int)operators.size();
		for (int i = 0; i < len; ++i) {
			if (i==len - 1) {
				operands[i]->llvmIrAnd(irBuilder.ctx.endBb);
			} else {
				bb = irBuilder.buildBb();
				operands[i]->llvmIrAnd(bb);
				irBuilder.setCurBb(bb);
				irBuilder.addBasicBlock(bb);
			}
		}

	}

	return nullptr;
}

Value *EqExp::llvmIr() {
	if (operands.empty()) {
		Value *val = leftOperand->llvmIr();
		if (val->type->isInt32()) {
			val = irBuilder.buildIcmpInst(IcmpType::NE, val, new ConstantInt(0));
		}
		return val;
	} else {
		Value *val = leftOperand->llvmIr();
		for (int i = 0; i < operators.size(); ++i) {
			if (val->type->isInt1())
				val = irBuilder.buildZext(val, IntegerType::INT32);
			Value *cmpVal = operands[i]->llvmIr();
			if (cmpVal->type->isInt1())
				cmpVal = irBuilder.buildZext(cmpVal, IntegerType::INT32);
			if (operators[i]==TokenType::EQL) {
				val = irBuilder.buildIcmpInst(IcmpType::EQ, val, cmpVal);
			}
			if (operators[i]==TokenType::NEQ) {
				val = irBuilder.buildIcmpInst(IcmpType::NE, val, cmpVal);
			}
		}
		return val;
	}
}

Value *FuncDef::llvmIr() {
	irBuilder.ctx.isGlobal = false;
	irBuilder.ctx.afterFuncDef = true;
	irBuilder.ctx.voidFunc = funcType.getType()==VOIDTK;
	symbol.startScope();

	Type *ty = funcType.getType()==INTTK ? IntegerType::INT32 : IntegerType::VOID;

	auto name = irBuilder.genFuncName(ident.getValue());
	auto *func = new Function(ty, name);
	irBuilder.addFunction(func);
	irBuilder.setCurFunc(func);

	vector<int> dims{};
	if (funcFParamsPtr) {
		dims = funcFParamsPtr->getDims();
	}
	symbol.insertFunc(ident.getValue(), funcType.getType()==VOIDTK, dims, func);

	auto bb = new BasicBlock(irBuilder.genBbName());
	irBuilder.addBasicBlock(bb);
	irBuilder.setCurBb(bb);

	if (funcFParamsPtr)
		funcFParamsPtr->llvmIr();
	blockPtr->llvmIr();

	irBuilder.fillInReturn();

	irBuilder.ctx.afterFuncDef = false;
	return nullptr;

}

int FuncFParam::getDim() {
	return (isArray) ? (int)expPtrs.size() + 1 : 0;
}


Value *FuncFParam::llvmIr() {
	vector<int> lens = {};
	if (isArray)
		lens.push_back(0);
	for (auto &i : expPtrs) {
		lens.push_back(i->evaluate());
	}

	Type *ty;
	if (isArray) {
		ty = new PointerType(IntegerType::INT32);
	} else {
		ty = IntegerType::INT32;
	}

	auto *arg = irBuilder.buildArgument(ident.getValue(), ty);
	if (!isArray) {
		auto *alloc = irBuilder.buildAlloc(ident.getValue(), IntegerType::INT32);
		irBuilder.buildStore(arg, alloc);
		symbol.insertVar(false, ident.getValue(), lens, vector<int>{}, alloc);
	} else {
		symbol.insertVar(false, ident.getValue(), lens, vector<int>{}, arg);
	}

	return nullptr;
}

vector<int> FuncFParams::getDims() {
	vector<int> dims{};
	for (const auto &i : funcFParamPtrs) {
		dims.push_back(i->getDim());
	}
	return dims;
}

Value *FuncFParams::llvmIr() {
	for (const auto &i : funcFParamPtrs) {
		i->llvmIr();
	}
	return nullptr;
}

vector<Value *> FuncRParams::llvmIrList() {
	vector<Value *> args{};
	args.reserve(expPtrs.size());
	for (const auto &i : expPtrs) {
		args.push_back(i->llvmIr());
	}
	return args;
}


Value *FunctionCall::llvmIr() {
	vector<Value *> args{};
	if (funcRParamsPtr)
		args = funcRParamsPtr->llvmIrList();
	auto funcPtr = symbol.getFunc(ident.getValue());
	auto *call = irBuilder.buildCall((Function *)funcPtr->getLlvmValue(), args);
	return call;
}

std::vector<int> ExpInitVal::evaluate() {
	return vector<int>{expPtr->evaluate()};
}
Value *ExpInitVal::llvmIr() {
	return expPtr->llvmIr();
}




std::vector<int> ArrayInitVal::evaluate() {
	vector<int> values{};
	for (const auto &i : initValPtrs) {
		vector<int> _ = i->evaluate();
		values.insert(values.end(), _.begin(), _.end());
	}
	return values;
}
vector<Value *> ArrayInitVal::llvmIrList() {
	vector<Value *> irList{};
	irList.reserve(initValPtrs.size());
	for (const auto &i : initValPtrs) {
		if (i->isArray()) {
			auto res = i->llvmIrList();
			irList.insert(irList.end(), res.begin(), res.end());
		} else {
			irList.push_back(i->llvmIr());
		}
	}
	return irList;
}

int LVal::evaluate() {
	vector<int> index{};
	index.reserve(array.size());
	for (const auto &i : array) {
		index.push_back(i->evaluate());
	}
	auto varPtr = symbol.getVar(ident.getValue());
	int res = varPtr->getValue(index);
	return res;
}

Value *LVal::llvmIr() {

	auto varSymbol = symbol.getVar(ident.getValue());
	Value *val = varSymbol->getLlvmValue();
	bool isPointer = array.size()!=varSymbol->lens.size();

	/// single-var
	if (varSymbol->lens.empty()) {
		if (varSymbol->isConst)
			return new ConstantInt(varSymbol->getValue(vector<int>{}));
		else {
			auto *load = irBuilder.buildLoad(val);
			return load;
		}
	} else if (isPointer) {
		Value *realOff = new ConstantInt(0);
		if (varSymbol->lens.size()==2 && array.size()==1) {
			Value *arrOff = array[0]->llvmIr();
			realOff = irBuilder.buildAlu(AluType::MUL, arrOff, new ConstantInt(varSymbol->lens[1]));
		}
		auto *gep = irBuilder.buildGEP(varSymbol->getLlvmValue(), realOff);
		return gep;

	} else {
		Value *dim1 = array.back()->llvmIr();
		/// @manual for 2 dimension
		if (array.size()==2) {
			Value *mul = irBuilder.buildAlu(
				AluType::MUL,
				array[0]->llvmIr(),
				new ConstantInt(varSymbol->lens.back()));
			dim1 = irBuilder.buildAlu(AluType::ADD, mul, dim1);
		}
		auto *gep = irBuilder.buildGEP(val, dim1);
		auto *load = irBuilder.buildLoad(gep);
		return load;
	}

	return nullptr;
}

Value *LVal::llvmIrAddr() {
	auto varSymbol = symbol.getVar(ident.getValue());
	Value *addr = varSymbol->getLlvmValue();

	if (array.empty()) {

	} else {
		Value *dim1 = array.back()->llvmIr();
		/// @manual for 2 dimension
		if (array.size()==2) {
			Value *mul = irBuilder.buildAlu(
				AluType::MUL,
				array[0]->llvmIr(),
				new ConstantInt(varSymbol->lens.back()));
			dim1 = irBuilder.buildAlu(AluType::ADD, mul, dim1);
		}
		addr = irBuilder.buildGEP(addr, dim1);
	}
	return addr;
}


Value *MainFuncDef::llvmIr() {
	irBuilder.ctx.isGlobal = false;
	irBuilder.ctx.afterFuncDef = true;
	irBuilder.ctx.voidFunc = false;
	symbol.startScope();

	Type *ty = bType.getType()==INTTK ? IntegerType::INT32 : IntegerType::VOID;
	auto name = irBuilder.genFuncName("main");
	auto *func = new Function(ty, name);
	irBuilder.addFunction(func);
	irBuilder.setCurFunc(func);

	symbol.insertFunc("main", bType.getType()==VOIDTK);

	auto bb = new BasicBlock(irBuilder.genBbName());
	irBuilder.addBasicBlock(bb);
	irBuilder.setCurBb(bb);

	blockPtr->llvmIr();

	irBuilder.ctx.afterFuncDef = false;
	return nullptr;
}

int PrimaryExp::evaluate() {
	if (expPtr)
		return expPtr->evaluate();
	else if (lValPtr)
		return lValPtr->evaluate();
	else if (numberPtr)
		return numberPtr->evaluate();
	return 0;
}

Value *PrimaryExp::llvmIr() {
	if (expPtr)
		return expPtr->llvmIr();
	else if (lValPtr)
		return lValPtr->llvmIr();
	else if (numberPtr)
		return numberPtr->llvmIr();
	return nullptr;
}

Value *AssignStmt::llvmIr() {
	auto *rvalue = expPtr->llvmIr();
	auto *lval = lValPtr->llvmIrAddr();
	irBuilder.buildStore(rvalue, lval);
	/// no need for sysY
	return nullptr;
}


Value *ExpStmt::llvmIr() {
	return expPtr->llvmIr();
}

Value *BreakStmt::llvmIr() {
	irBuilder.buildBrInst(irBuilder.endStack.top());
	return nullptr;
}

Value *ContinueStmt::llvmIr() {
	irBuilder.buildBrInst(irBuilder.incStack.top());
	return nullptr;
}

Value *ReturnStmt::llvmIr() {
	if (expPtr) {
		auto *ret = expPtr->llvmIr();
		irBuilder.buildReturn(ret);
	} else
		irBuilder.buildReturn(new Value(IntegerType::VOID, ""));
	return nullptr;
}

Value *GetintStmt::llvmIr() {
	auto *rvalue = irBuilder.buildCall(Function::getint, vector<Value *>{});
	auto *lval = lValPtr->llvmIrAddr();
	irBuilder.buildStore(rvalue, lval);
	/// no need for sysY
	return nullptr;
}

vector<string> splitStrings(const string &str) {
	std::stringstream ss(str.substr(1, str.size() - 2));
	std::vector<std::string> tokens;

	std::string token;
	while (getline(ss, token, '%')) {
		tokens.push_back({token + "\0"});
		if (ss.peek()=='d') {
			tokens.emplace_back("%d");
			ss.ignore();
		}
	}
	tokens.erase(remove(tokens.begin(), tokens.end(), ""), tokens.end());

	return tokens;
}

Value *PrintfStmt::llvmIr() {
	vector<Value *> args{};
	args.reserve(expPtrs.size());
	for (auto &i : expPtrs) {
		args.push_back(i->llvmIr());
	}

	vector<string> strings = splitStrings(formatString.getValue());
	int index = 0;
	for (auto &i : strings) {
		if (i=="%d") {
			irBuilder.buildCall(Function::putint, vector<Value *>{args[index++]});
		} else {
			auto *str = new ConstantString(i);
			auto *g = irBuilder.buildGlobalVar("", str, true);
			auto *gep = irBuilder.buildGEP(g, new ConstantInt(0));
			irBuilder.buildCall(Function::putstr, vector<Value *>{gep});
		}
	}
	return nullptr;
}

int UnaryExp::evaluate() {
	/// when evaluating in global variable analysis,
	/// primaryExp is the \only possible one
	int res = primaryExpPtr->evaluate();
	size_t negationCount = std::count(unaryOps.begin(), unaryOps.end(), MINU);
	return (negationCount%2) ? -res : res;
}

Value *UnaryExp::llvmIr() {
	Value *ret;
	if (primaryExpPtr)
		ret = primaryExpPtr->llvmIr();
	else if (functionCallPtr)
		ret = functionCallPtr->llvmIr();

	size_t negationCount = std::count(unaryOps.begin(), unaryOps.end(), MINU);
	if (negationCount%2) {
		ret = irBuilder.buildAlu(AluType::SUB, new ConstantInt(0), ret);
	}
	size_t notCount = std::count(unaryOps.begin(), unaryOps.end(), NOT);
	if (notCount%2) {
		ret = irBuilder.buildIcmpInst(IcmpType::EQ, ret, new ConstantInt(0));
		ret = irBuilder.buildZext(ret, IntegerType::INT32);
	}
	return ret;
}










































