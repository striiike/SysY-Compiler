cmake_minimum_required(VERSION 3.15)
project(compiler)

set(CMAKE_CXX_STANDARD 17)

add_executable(compiler main.cpp
        frontend/lexer/Lexer.cpp
        frontend/lexer/Lexer.h
        frontend/parser/Parser.h
        config.h
        frontend/parser/TokenStream.h
        frontend/nodes/ASTNode.h
        frontend/nodes/ExpUniform.h
        frontend/nodes/Exp.h
        frontend/nodes/TokenNode.h
        frontend/nodes/Decl.h
        frontend/nodes/Decl.h
        frontend/nodes/Def.h
        frontend/nodes/InitVal.h
        frontend/nodes/BlockItem.h
        frontend/nodes/SimpleStmt.h
        frontend/nodes/ComplexStmt.h
        frontend/nodes/Stmt.h
        frontend/nodes/FuncDef.h
        frontend/nodes/MainFuncDef.h
        frontend/nodes/FuncFParams.h
        frontend/nodes/CompUnit.h
        frontend/nodes/lib.h
        frontend/parser/Parser.cpp
        frontend/parser/Exception.h
        frontend/parser/Symbol.h
        frontend/parser/Symbol.cpp
        frontend/nodes/CompUnit.h
        frontend/nodes/UnaryExp.h
        frontend/nodes/PrimaryExp.h
        frontend/nodes/LVal.h
        frontend/nodes/FunctionCall.h
        llvm-ir/Module.h
        llvm-ir/Type.h
        llvm-ir/Function.h
        llvm-ir/Value.h
        llvm-ir/User.h
        llvm-ir/instruction/Instruction.h
        llvm-ir/constant/Constant.h
        llvm-ir/BasicBlock.h
        llvm-ir/GlobalVariable.h
        llvm-ir/Use.h
        llvm-ir/IrBuilder.h
        llvm-ir/IrBuilder.cpp
        llvm-ir/instruction/ReturnInst.h
        frontend/nodes/Block.h
        llvm-ir/instruction/AllocaInst.h
        llvm-ir/instruction/AluInst.h
        llvm-ir/constant/ConstantInt.h
        llvm-ir/constant/ConstantArray.h
        llvm-ir/instruction/StoreInst.h
        llvm-ir/instruction/GEPInst.h
        llvm-ir/instruction/LoadInst.h
        llvm-ir/instruction/CallInst.h
        llvm-ir/constant/ConstantString.h
        llvm-ir/instruction/BrInst.h
        llvm-ir/instruction/IcmpInst.h
        llvm-ir/instruction/ZextInst.h
        midend/DFBuilder.h
        midend/DFBuilder.cpp
        midend/MidEnd.h
        midend/Mem2Reg.h
        midend/Mem2Reg.cpp
        llvm-ir/instruction/PhiInst.h
        llvm-ir/Value.cpp
        llvm-ir/User.cpp
        backend/mips/MipsInst.h
        backend/MipsParser.h
        backend/mips/MipsComponent.cpp
        backend/mips/MipsInst.cpp
        backend/mips/MipsOperand.cpp
        backend/MipsParser.cpp
#        llvm-ir/BasicBlock.cpp
#        llvm-ir/Function.cpp
#        llvm-ir/Module.cpp
#        llvm-ir/constant/Constant.cpp
        frontend/CheckError.cpp
        frontend/LlvmIr.cpp
        midend/RemovePhi.h
        midend/RemovePhi.cpp
        llvm-ir/instruction/ParallelCopyInst.h
        backend/MipsAllocator.h
        backend/MipsAllocator.cpp
        midend/DeadCodeEmit.h
        midend/DeadCodeEmit.cpp
        midend/GVN.h
        midend/GVN.cpp
        backend/PeepHole.h
        backend/PeepHole.cpp
)
