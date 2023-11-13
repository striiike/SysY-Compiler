//
// Created by hiccup on 2023/10/20.
//

#ifndef COMPILER_LLVM_IR_IRBUILDER_H
#define COMPILER_LLVM_IR_IRBUILDER_H

#include <vector>
#include <memory>
#include <stack>
#include "Function.h"
#include "Module.h"
#include "constant/Constant.h"
#include "constant/ConstantInt.h"
#include "constant/ConstantArray.h"
#include "constant/ConstantString.h"
#include "instruction/AllocaInst.h"
#include "instruction/StoreInst.h"
#include "instruction/GEPInst.h"
#include "instruction/AluInst.h"
#include "instruction/ReturnInst.h"
#include "instruction/LoadInst.h"
#include "instruction/CallInst.h"
#include "instruction/ZextInst.h"
#include "instruction/BrInst.h"
#include "instruction/IcmpInst.h"
#include "instruction/PhiInst.h"

using namespace std;

struct IrCtx {
	bool isConst;
	bool isGlobal;
	bool isLeftValue;
	bool voidFunc;
	bool afterFuncDef;
	int loopNum = 0;
	int layerNum = 0;

	BasicBlock *thenBb, *condBb, *endBb;
};

struct IrRet {
	bool hasRet;
	int retLineNum;
	int rbraceLineNum;
	int dim;
	bool undefined = false;

	vector<int> dims{};
	//    ErrorRet()
};

using IrCtxPtr = std::shared_ptr<IrCtx>;
using IrRetPtr = std::shared_ptr<IrRet>;

static std::string globalPrefix = "@g_";
static std::string funcPrefix = "@f_";
static std::string localVarPrefix = "%t_";
static std::string bbPrefix = "b";
static std::string argPrefix = "%a_";

class IrBuilder {
	int bbCnt;
	int argsCnt;
	int globalVarCnt;
	int strLiteralCnt;
	int localVarCnt;

	Function *curFunc{};
	BasicBlock *curBb{};
	Module *module{};
public:
	IrCtx ctx = IrCtx();

	std::stack<BasicBlock *> incStack{};
	std::stack<BasicBlock *> endStack{};

	IrBuilder()
		: bbCnt(0), argsCnt(0), globalVarCnt(0), strLiteralCnt(0), localVarCnt(0) {}

	void addArgument(Argument *arg) {
		arg->setFunction(curFunc);
		curFunc->addArgument(arg);
	}

	void addFunction(Function *func);

	void addBasicBlock(BasicBlock *bb);

	void addGlobalVariable(GlobalVariable *globalVar);

	std::string genFuncName(const std::string &name);

	std::string genBbName();

	std::string genGlobalVarName(const std::string &name = "");

	std::string genLocalVarName(const std::string &name = "");

	std::string genArgName(const std::string &name = "");

	Function *getCurFunc() const { return curFunc; }

	void setCurFunc(Function *CurFunc) { curFunc = CurFunc; }

	BasicBlock *getCurBb() const { return curBb; }

	void setCurBb(BasicBlock *CurBb) { curBb = CurBb; }

	Module *getModule() const { return module; }

	void setModule(Module *Module) { module = Module; }

	BasicBlock *buildBb();

	Value *buildGlobalVar(const string &ident, Constant *constant, bool isConst);

	Value *buildArgument(const string &ident, Type *ty);

	// for mem2reg
	Value *buildAlloc(const string &ident, Type *ty);

	void buildStore(Value *content, Value *addr);

	Value *buildLoad(Value *addr);

	Value *buildGEP(Value *base, Value *ptrOff, Value *arrOff = nullptr);

	/// contains @add, @sub, @sidv, @srem, @mul
	Value *buildAlu(AluType ty, Value *op1, Value *op2);

	Value *buildReturn(Value *ret);

	Value *buildCall(Function *func, vector<Value *> args);

	Value *buildZext(Value *val, Type *tar);

	Value *buildIcmpInst(IcmpType ty, Value *op1, Value *op2);

	Value *buildBrInst(Value *cond, BasicBlock *bb1, BasicBlock *bb2);

	Value *buildBrInst(BasicBlock *bb);

	Value *buildPhiInst(vector<BasicBlock*> *bbs, BasicBlock *to);

	void fillInReturn();
};

extern IrBuilder irBuilder;

#endif //COMPILER_LLVM_IR_IRBUILDER_H
