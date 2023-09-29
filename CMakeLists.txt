cmake_minimum_required(VERSION 3.26)
project(compiler)

set(CMAKE_CXX_STANDARD 17)

add_executable(compiler main.cpp
        lexer/Lexer.cpp
        lexer/Lexer.h
        lexer/Lexer.cpp
        parser/Parser.hpp
        Config.h
        Config.cpp
        parser/TokenStream.hpp
        parser/TokenStream.cpp
        parser/ASTNode.hpp
        parser/expression/Uniform.hpp
        parser/expression/ExpressionParser.hpp
        parser/expression/ExpressionParser.cpp
        parser/expression/other.hpp
        parser/expression/other.cpp
        parser/TokenNode.hpp
        parser/define.hpp
        parser/declaration/DeclarationParser.hpp
        parser/declaration/DeclarationParser.cpp
        parser/declaration/Decl.hpp
        parser/declaration/Decl.hpp
        parser/declaration/Def.hpp
        parser/declaration/InitVal.hpp
        parser/statement/StatementParser.hpp
        parser/statement/StatementParser.cpp
        parser/function/FunctionParser.hpp
        parser/function/FunctionParser.cpp
        parser/statement/BlockItem.hpp
        parser/statement/SimpleStmt.hpp
        parser/statement/ComplexStmt.hpp
        parser/statement/Stmt.hpp
        parser/statement/StatementParser.cpp
        parser/statement/StatementParser.hpp
        parser/function/FuncDef.hpp
        parser/function/MainFuncDef.hpp
        parser/function/FuncFParams.hpp
        parser/CompUnit.hpp
)
