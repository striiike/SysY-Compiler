//
// Created by hiccup on 2023/9/25.
//

#ifndef COMPILER_DEF_H
#define COMPILER_DEF_H

#include "TokenNode.h"
#include "Exp.h"
#include "InitVal.h"
#include "../parser/Symbol.hpp"

class Def : public ASTNode {
	bool isConst;
	TokenNode ident;
	std::vector<ExpPtr> expPtrs;
	InitValPtr initValPtr;
public:
	Def(bool isConst, TokenNode ident, std::vector<ExpPtr> expPtrs, InitValPtr initValPtr);

	void checkError(ErrorCtxPtr ctx, ErrorRetPtr ret) override;

};

#endif //COMPILER_DEF_H
