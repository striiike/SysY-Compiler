//
// Created by hiccup on 2023/9/30.
//

#pragma once

#include <vector>
#include <utility>
#include <memory>

using namespace std;

enum class Exception {
	INVALID_CHARACTER,  //
	REDEFINED_IDENT,
	UNDEFINED_IDENT,
	PARAMS_NUM_UNMATCHED,
	PARAM_TYPE_UNMATCHED,
	VOID_RETURN_INT,
	INT_RETURN_LACKED,
	CONST_ASSIGNED,
	SEMICN_LACKED,  // #
	RPARENT_LACKED, // #
	RBRACK_LACKED,  // #
	FORMAT_CHAR_UNMATCHED,
	BREAK_CONTINUE_OUT_LOOP,
	UNDEFINED_ERROR,
	CORRECT
};

struct ErrorCtx {
	bool isConst;
	bool isGlobal;
	bool isLeftValue;
	bool voidFunc;
	bool afterFuncDef;
	int loopNum = 0;
	int layerNum = 0;
};

struct ErrorRet {
	bool hasRet;
	int retLineNum;
	int rbraceLineNum;
	int dim;
	bool undefined = false;

	vector<int> dims{};
	//    ErrorRet()
};

using ErrorCtxPtr = std::shared_ptr<ErrorCtx>;
using ErrorRetPtr = std::shared_ptr<ErrorRet>;
//using ErrorCtxPtr = ErrorCtx *;
//using ErrorRetPtr = ErrorRet *;

