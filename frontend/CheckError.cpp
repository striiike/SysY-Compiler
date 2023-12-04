//
// Created by hiccup on 2023/11/21.
//
#include "nodes/lib.h"
#include "parser/Symbol.h"

void UnaryExp::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	auto _ret = make_shared<ErrorRet>();
	if (primaryExpPtr) {
		primaryExpPtr->checkError(ctx, _ret);
	} else if (functionCallPtr) {
		functionCallPtr->checkError(ctx, _ret);
	}
	ret->dim = _ret->dim;
}

void AssignStmt::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	ctx->isLeftValue = true;
	lValPtr->checkError(ctx, ret);
	ctx->isLeftValue = false;
	expPtr->checkError(ctx, ret);
}

void ExpStmt::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	expPtr->checkError(ctx, ret);
}

void BreakStmt::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	if (!ctx->loopNum)
		errorList.emplace_back(Exception::BREAK_CONTINUE_OUT_LOOP, _break.getLineNum());
}

void ContinueStmt::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	if (!ctx->loopNum)
		errorList.emplace_back(Exception::BREAK_CONTINUE_OUT_LOOP, _continue.getLineNum());
}

void ReturnStmt::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	if (expPtr) {
		expPtr->checkError(ctx, ret);
		if (ctx->voidFunc) {
			errorList.emplace_back(Exception::VOID_RETURN_INT, _return.getLineNum());
		}
	}
	ret->hasRet = true;
	ret->retLineNum = _return.getLineNum();
}

void GetintStmt::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	ctx->isLeftValue = true;
	lValPtr->checkError(ctx, ret);
	ctx->isLeftValue = false;
}

void PrintfStmt::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	auto str = formatString.getValue();

	// skip " "
	bool formatUnmatched = false;
	int cnt = 0;
	for (int i = 1; i < str.size() - 1; i++) {
		if (str[i]=='\\' && str[i + 1]!='n')
			formatUnmatched = true;
		if (str[i] != '\n' && str[i]!=32 && str[i]!=33 && (str[i] < 40 || str[i] > 126) && str[i]!='%')
			formatUnmatched = true;
		if (str[i]=='%') {
			if (str[i + 1]=='d')
				cnt++;
			else
				formatUnmatched = true;
		}
	}
	if (formatUnmatched)
		errorList.emplace_back(Exception::INVALID_CHARACTER, formatString.getLineNum());
	if (cnt!=expPtrs.size())
		errorList.emplace_back(Exception::FORMAT_CHAR_UNMATCHED, _printf.getLineNum());
	for (const auto &i : expPtrs)
		i->checkError(ctx, ret);
}

void PrimaryExp::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	auto _ret = make_shared<ErrorRet>();

	if (expPtr)
		expPtr->checkError(ctx, _ret);
	else if (lValPtr)
		lValPtr->checkError(ctx, _ret);
	else if (numberPtr)
		numberPtr->checkError(ctx, _ret);

	ret->dim = _ret->dim;
}

void MainFuncDef::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	ctx->afterFuncDef = true;
	ctx->voidFunc = false;
	symbol.startScope();

	ret = make_shared<ErrorRet>();
	blockPtr->checkError(ctx, ret);
	ctx->afterFuncDef = false;

	if (!ret->hasRet) {
		errorList.emplace_back(Exception::INT_RETURN_LACKED, ret->rbraceLineNum);
	}
}

void LVal::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	auto varPtr = symbol.getVar(ident.getValue());
	vector<int> lens{};
	if (varPtr)
		lens = varPtr->lens;
	if (!varPtr) {
		errorList.emplace_back(Exception::UNDEFINED_IDENT, ident.getLineNum());
		ret->undefined = true;
	} else if (varPtr->isConst && ctx->isLeftValue)
		errorList.emplace_back(Exception::CONST_ASSIGNED, ident.getLineNum());

	ctx->isLeftValue = false;

	for (const auto &i : array) {
		i->checkError(ctx, ret);
	}

	ret->dim = (int)lens.size() - (int)array.size();
}

void ExpInitVal::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	expPtr->checkError(ctx, ret);
}

void ArrayInitVal::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	for (const auto &i : initValPtrs) {
		i->checkError(ctx, ret);
	}
}

void FuncRParams::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	vector<int> dims{};
	for (const auto &i : expPtrs) {
		auto _ret = make_shared<ErrorRet>();
		i->checkError(ctx, _ret);
		dims.push_back(_ret->dim);
	}
	ret->dims = dims;
}

void FunctionCall::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	auto funcPtr = symbol.getFunc(ident.getValue());

	vector<int> dims{};
	if (funcPtr) {
		dims = funcPtr->dims;
		ret->dim = (funcPtr->isVoid) ? -1 : 0;
	} else {
		errorList.emplace_back(Exception::UNDEFINED_IDENT, ident.getLineNum());
		ret->dim = 0;
	}

	if (funcRParamsPtr)
		funcRParamsPtr->checkError(ctx, ret);

	if (dims.size()!=ret->dims.size())
		errorList.emplace_back(Exception::PARAMS_NUM_UNMATCHED, ident.getLineNum());
	else if (!ret->undefined) {
		for (int i = 0; i < dims.size(); ++i) {
			if (dims[i]!=ret->dims[i]) {
				errorList.emplace_back(Exception::PARAM_TYPE_UNMATCHED, ident.getLineNum());
				break;
			}
		}
	}
}

void FuncFParams::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	for (const auto &i : funcFParamPtrs) {
		i->checkError(ctx, ret);
	}
//    symbol.endScope();
}

void FuncFParam::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	for (const auto &i : expPtrs) {
		i->checkError(ctx, ret);
	}
	vector<int> lens((isArray) ? expPtrs.size() + 1 : 0);

	if (!symbol.insertVar(false, ident.getValue(), lens)) {
		errorList.emplace_back(Exception::REDEFINED_IDENT, ident.getLineNum());
	}
}

void FuncDef::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	vector<int> dims{};
	if (funcFParamsPtr) {
		dims = funcFParamsPtr->getDims();
	}
	// no detail for check error
	if (!symbol.insertFunc(ident.getValue(), funcType.getType()==VOIDTK, dims)) {
		errorList.emplace_back(Exception::REDEFINED_IDENT, ident.getLineNum());
	}

	symbol.startScope();
	ret = make_shared<ErrorRet>();
	if (funcFParamsPtr)
		funcFParamsPtr->checkError(ctx, ret);
	ctx->afterFuncDef = true;
	ctx->voidFunc = (funcType.getType()==VOIDTK);
	blockPtr->checkError(ctx, ret);

	if (!ctx->voidFunc && !ret->hasRet) {
		errorList.emplace_back(Exception::INT_RETURN_LACKED, ret->rbraceLineNum);
	}
}

void Number::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	ret->dim = 0;
}

void Exp::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	addExpPtr->checkError(ctx, ret);
}

void Cond::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	lOrExpPtr->checkError(ctx, ret);
}

void Def::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	for (auto &i : expPtrs) {
		i->checkError(ctx, ret);
	}

	if (initValPtr)
		initValPtr->checkError(ctx, ret);
	// no detail
	auto lens = vector<int>(expPtrs.size());
	if (!symbol.insertVar(isConst, ident.getValue(), lens)) {
		errorList.emplace_back(Exception::REDEFINED_IDENT, ident.getLineNum());
	}
}

void Decl::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	ctx->isConst = this->isConst;
	for (const auto &i : defPtrs)
		i->checkError(ctx, ret);
	ctx->isConst = false;
}

void CompUnit::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	symbol.startScope();
	for (const auto &i : declPtrs)
		i->checkError(ctx, ret);
	for (const auto &i : funcDefPtrs)
		i->checkError(ctx, ret);
	mainFuncDefPtr->checkError(ctx, ret);
	symbol.endScope();
}

void IfStmt::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	condPtr->checkError(ctx, ret);
	stmtPtr->checkError(ctx, ret);
	if (stmtElsePtr)
		stmtElsePtr->checkError(ctx, ret);
}

void _ForStmt::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	ctx->isLeftValue = true;
	lValPtr->checkError(ctx, ret);
	ctx->isLeftValue = false;
	expPtr->checkError(ctx, ret);
}

void ForStmt::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	if (assignStmtPtr1)
		assignStmtPtr1->checkError(ctx, ret);
	if (condPtr)
		condPtr->checkError(ctx, ret);
	if (assignStmtPtr2)
		assignStmtPtr2->checkError(ctx, ret);

	ctx->loopNum++;
	stmtPtr->checkError(ctx, ret);
	ctx->loopNum--;
}

void BlockItem::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	if (declPtr)
		declPtr->checkError(ctx, ret);
	if (stmtPtr)
		stmtPtr->checkError(ctx, ret);
}

void Block::checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) {
	if (!ctx->afterFuncDef)
		symbol.startScope();
	else
		ctx->afterFuncDef = false;

	ctx->layerNum++;
	for (const auto &i : blockItemPtrs) {
		i->checkError(ctx, ret);
	}
	ctx->layerNum--;

	symbol.endScope();
	ret->rbraceLineNum = rbrace.getLineNum();
}

