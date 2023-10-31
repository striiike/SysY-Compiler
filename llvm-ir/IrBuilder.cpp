//
// Created by hiccup on 2023/10/20.
//

#include "IrBuilder.h"

IrBuilder irBuilder{};

IntegerType *IntegerType::VOID = new IntegerType(0);
IntegerType *IntegerType::INT1 = new IntegerType(1);
IntegerType *IntegerType::INT8 = new IntegerType(8);
IntegerType *IntegerType::INT32 = new IntegerType(32);

Function *Function::getint = new Function(IntegerType::INT32, "@getint");
Function *Function::putch = new Function(IntegerType::VOID, "@putch");
Function *Function::putint = new Function(IntegerType::VOID, "@putint");
Function *Function::putstr = new Function(IntegerType::VOID, "@putstr");
