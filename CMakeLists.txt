cmake_minimum_required(VERSION 3.26)
project(compiler)

set(CMAKE_CXX_STANDARD 17)

add_executable(compiler main.cpp
        lexer/Lexer.cpp
        lexer/Lexer.h
        parser/Parser.hpp
        Config.h
        parser/TokenStream.hpp
        parser/TokenStream.cpp
        nodes/ASTNode.hpp
        nodes/ExprUniform.hpp
        nodes/ExprOthers.hpp
        nodes/ExprOthers.cpp
        parser/TokenNode.hpp
        nodes/Decl.hpp
        nodes/Decl.hpp
        nodes/Def.hpp
        nodes/InitVal.hpp
        nodes/BlockItem.hpp
        nodes/SimpleStmt.hpp
        nodes/ComplexStmt.hpp
        nodes/Stmt.hpp
        nodes/FuncDef.hpp
        nodes/MainFuncDef.hpp
        nodes/FuncFParams.hpp
        nodes/CompUnit.hpp
        nodes/lib.hpp
        parser/Parser.cpp
)
