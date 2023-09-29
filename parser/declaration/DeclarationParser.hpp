//
// Created by hiccup on 2023/9/24.
//

#pragma once

#include <memory>
#include "../TokenStream.hpp"
#include "../define.hpp"
#include "Def.hpp"
#include "Decl.hpp"
#include "InitVal.hpp"

//    <BType>         := 'int'
//    <Decl>          := ['const'] <BType> <Def> { ',' <Def> } ';'    // 'const' 修饰若有，则表示常量
//    <Def>           := Ident { '[' <ConstExp> ']'  } [ '=' <InitVal> ]   // 如果是常量声明则必须有 InitVal
//    <InitVal>       := <ExpInitVal> | <ArrInitVal>
//    <ExpInitVal>    := <Exp>
//    <ArrInitVal>    := '{' [ <InitVal> { ',' <InitVal> } ] '}'    // 语义分析时要求必须个数与维度对应

class DeclarationParser {
private:
    TokenStream &tokenStream;
public:
    explicit DeclarationParser(TokenStream &tokenStream);

    std::shared_ptr<Decl> parseDecl();

    std::shared_ptr<Def> parseDef(bool isConst);

    InitValPtr parseInitVal(bool isConst);

    ExpInitValPtr parseExpInitVal(bool isConst);

    ArrayInitValPtr parseArrayInitVal(bool isConst);
};


