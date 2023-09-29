//
// Created by hiccup on 2023/9/26.
//

#pragma once


#include "../TokenStream.hpp"
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

