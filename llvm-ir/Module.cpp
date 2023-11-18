//
// Created by hiccup on 2023/10/20.
//

#include "Module.h"
Module::Module() {
	Function::putch->addArgument(new Argument(IntegerType::INT32, "!ub!"));
	Function::putint->addArgument(new Argument(IntegerType::INT32, "!ub!"));
	Function::putstr->addArgument(new Argument(new PointerType(IntegerType::INT8), "!ub!"));

	Function::getint->isLink = true;
	Function::putch->isLink = true;
	Function::putint->isLink = true;
	Function::putstr->isLink = true;
}
void Module::addGlobalVariable(GlobalVariable *globalVar) {
	globalList.push_back(globalVar);
}
void Module::addFunction(Function *func) {
	functionList.push_back(func);
}
std::string Module::toString() {
	std::string str;
	str += Function::getint->toString() + "\n";
	str += Function::putch->toString() + "\n";
	str += Function::putint->toString() + "\n";
	str += Function::putstr->toString() + "\n\n";
	for (auto i : globalList) {
		str += i->toString();
		str += "\n";
	}
	for (auto i : functionList) {
		str += i->toString();
		str += "\n";
	}
	return str;
}
