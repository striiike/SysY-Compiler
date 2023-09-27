cmake_minimum_required(VERSION 3.26)
project(compiler)

set(CMAKE_CXX_STANDARD 17)

add_executable(compiler main.cpp
        lexer/Lexer.cpp
        lexer/Lexer.h
        lexer/Lexer.cpp
        parser/Parser.h
        parser/Parser.cpp
        driver/Driver.h
        driver/Driver.cpp
        driver/Config.h
        driver/Config.cpp
        parser/TokenStream.h
        parser/TokenStream.cpp
        parser/ASTNode.hpp
        parser/ASTNode.cpp
        parser/expression/uniform/Uniform.hpp
        parser/expression/ExpressionParser.hpp
        parser/expression/ExpressionParser.cpp
        parser/expression/other/other.hpp
        parser/expression/other/other.cpp
        parser/TokenNode.hpp
        parser/define.hpp
        parser/declaration/DeclarationParser.hpp
        parser/declaration/DeclarationParser.cpp
        parser/declaration/Decl.hpp
        parser/declaration/Decl.hpp
        parser/declaration/Def.hpp
        parser/declaration/InitVal.hpp
        parser/declaration/Def.cpp
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
