//
// Created by hiccup on 2023/10/1.
//
#include "Symbol.hpp"

Symbol symbol;

vector<pair<Exception, int>> errorList{};

VarSymbol::VarSymbol(bool isConst, bool isGlobal, vector<int> lens, vector<int> values, Value *llvm)
	: isConst(isConst), isGlobal(isGlobal),
	  lens(std::move(lens)), values(std::move(values)),
	  llvmValue(llvm) {}

int VarSymbol::getValue(const vector<int> &index) {
	if (values.empty())
		return 0;
	if (index.empty())
		return values[0];

	/// \manual for two dim
	int coordinate = index.back();
	if (index.size()==2)
		coordinate += index[0]*lens[0];

	return values[coordinate];
}

Value *VarSymbol::getLlvmValue() const {
	return llvmValue;
}

FuncSymbol::FuncSymbol(bool isVoid, vector<int> dims, Value *llvmValue) :
	isVoid(isVoid), dims(std::move(dims)), llvmValue(llvmValue) {}

Value *FuncSymbol::getLlvmValue() const {
	return llvmValue;
}

int Symbol::insertVar(bool isConst,
					  const string &name,
					  const vector<int> &lens,
					  const vector<int> &values,
					  Value *llvm) {
	auto scope = symbolStack.top();
	if (scope->count(name))
		return 0;

	bool isGlobal = symbolStack.size()==1;
	if (isGlobal && funcsMap.count(name))
		return 0;

	scope->insert(name);
	if (!varsMap.count(name))
		varsMap[name] = make_shared<stack<VarSymbolPtr>>();

	varsMap[name]->push(make_shared<VarSymbol>(isConst, isGlobal, lens, values, llvm));
	return 1;
}

int Symbol::insertFunc(const string &name, bool isVoid, const vector<int> &dims, Value *func) {
	auto scope = symbolStack.top();
	if (funcsMap.count(name) || scope->count(name))
		return 0;

	funcsMap[name] = make_shared<FuncSymbol>(isVoid, dims, func);
	return 1;
}

void Symbol::startScope() {
	symbolStack.push(make_shared<set<string>>());
}

void Symbol::endScope() {
	SymbolSetPtr set = symbolStack.top();

	for (const auto &i : *set) {
		varsMap[i]->pop();
		if (varsMap[i]->empty())
			varsMap.erase(i);
	}

	symbolStack.pop();
}

VarSymbolPtr Symbol::getVar(const string &name) {
	if (!varsMap.count(name))
		return nullptr;
	return varsMap[name]->top();
}

FuncSymbolPtr Symbol::getFunc(const string &name) {
	if (!funcsMap.count(name))
		return nullptr;
	return funcsMap[name];
}

void Symbol::clear() {
	while (!symbolStack.empty())
		symbolStack.pop();

	varsMap.clear();
	funcsMap.clear();
}
