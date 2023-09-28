//
// Created by hiccup on 2023/9/26.
//

#ifndef COMPILER_STATEMENTPARSER_HPP
#define COMPILER_STATEMENTPARSER_HPP


#include "../TokenStream.h"
#include "../define.hpp"

#include "BlockItem.hpp"
#include "ComplexStmt.hpp"
#include "SimpleStmt.hpp"
#include "Stmt.hpp"

class StatementParser {
private:
    TokenStream &tokenStream;
public :
    explicit StatementParser(TokenStream &tokenStream);

    BlockPtr parseBlock();

    BlockItemPtr parseBlockItem();

    StmtPtr parseStmt();

    SimpleStmtPtr parseSimpleStmt();

    ComplexStmtPtr parseComplexStmt();

    _ForStmtPtr parse_ForStmt();

    SimpleStmtPtr parseAssignOrGetintStmt(const LValPtr& lValPtr);
};


#endif //COMPILER_STATEMENTPARSER_HPP
