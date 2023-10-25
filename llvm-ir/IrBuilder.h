//
// Created by hiccup on 2023/10/20.
//

#ifndef COMPILER_LLVM_IR_IRBUILDER_H
#define COMPILER_LLVM_IR_IRBUILDER_H

#include <vector>
#include <memory>
#include "Function.h"
#include "Module.h"
using namespace std;

struct IrCtx {
	bool isConst;
	bool isGlobal;
	bool isLeftValue;
	bool voidFunc;
	bool afterFuncDef;
	int loopNum = 0;
	int layerNum = 0;
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
static std::string localVarPrefix = "tmp_";
static std::string bbPrefix = "b";

class IrBuilder {
	int bbCnt;
	int argsCnt;
	int globalVarCnt;
	int strLiteralCnt;

	Function *curFunc{};
	BasicBlock *curBb{};
	Module *module{};
public:
	IrCtx ctx = IrCtx();

	IrBuilder()
		: bbCnt(0), argsCnt(0), globalVarCnt(0), strLiteralCnt(0) {}

	void addArgument(Argument *arg) {
		arg->setFunction(curFunc);
		curFunc->addArgument(arg);
	}

	void addFunction(Function *func) {
		module->addFunction(func);
	}

	void addBasicBlock(BasicBlock *bb) {
		curFunc->addBasicBlock(bb);
		bb->setParent(curFunc);
	}

	void addGlobalVariable(GlobalVariable *globalVar) {
		module->addGlobalVariable(globalVar);
	}

	std::string genFuncName(const std::string &name) {
		if (name == "main") {
			return "@main";
		}
		return funcPrefix + name;
	}

	std::string genBbName() {
		return bbPrefix + "_" + to_string(bbCnt++);
	}

	std::string genGlobalVarName(const std::string &name) {
		return globalPrefix + name + "_" + to_string((globalVarCnt++));
	}

	Function *getCurFunc() const { return curFunc; }

	void setCurFunc(Function *CurFunc) { curFunc = CurFunc; }

	BasicBlock *getCurBb() const { return curBb; }

	void setCurBb(BasicBlock *CurBb) { curBb = CurBb; }

	Module *getModule() const { return module; }

	void setModule(Module *Module) { module = Module; }

};

extern IrBuilder irBuilder;

#endif //COMPILER_LLVM_IR_IRBUILDER_H
