//
// Created by hiccup on 2023/10/1.
//

#pragma once

#include <utility>
#include "Exception.hpp"
#include <set>
#include <stack>
#include <map>
#include <iostream>

using namespace std;

class VarSymbol {
public:
	bool isConst;
	bool isGlobal;
	vector<int> lens;  // at most 2 dimensions
	vector<int> values;     // save it in 1 dimension

	VarSymbol(bool isConst, bool isGlobal, vector<int> lens, vector<int> values) :
		isConst(isConst), isGlobal(isGlobal), lens(std::move(lens)), values(std::move(values)) {}

};

class FuncSymbol {
public:
	bool isVoid;
	vector<int> dims;       // stand for the types

	FuncSymbol(bool isVoid, vector<int> dims) :
		isVoid(isVoid), dims(std::move(dims)) {}
};

using SymbolSetPtr = shared_ptr<set<string>>;
using VarSymbolPtr = shared_ptr<VarSymbol>;
using FuncSymbolPtr = shared_ptr<FuncSymbol>;

class Symbol {
	stack<SymbolSetPtr> symbolStack;
	map<string, shared_ptr<stack<VarSymbolPtr>>> varsMap;
	map<string, FuncSymbolPtr> funcsMap;
public:
	Symbol() = default;

	// 0 for false
	int insertVar(bool isConst, const string &name, const vector<int> &lens, const vector<int> &values = {}) {
		auto scope = symbolStack.top();
		if (scope->count(name))
			return 0;

		bool isGlobal = symbolStack.size() == 1;
		if (isGlobal && funcsMap.count(name))
			return 0;

		scope->insert(name);
		if (!varsMap.count(name))
			varsMap[name] = make_shared<stack<VarSymbolPtr>>();

		varsMap[name]->push(make_shared<VarSymbol>(isConst, isGlobal, lens, values));
		return 1;
	}

	// 0 for false
	int insertFunc(const string &name, bool isVoid, const vector<int> &dims = {}) {
		auto scope = symbolStack.top();
		if (funcsMap.count(name) || scope->count(name))
			return 0;

		funcsMap[name] = make_shared<FuncSymbol>(isVoid, dims);
		return 1;
	}

	void startScope() {
		symbolStack.push(make_shared<set<string>>());
	}

	void endScope() {
		SymbolSetPtr set = symbolStack.top();

		for (const auto &i : *set) {
			varsMap[i]->pop();
			if (varsMap[i]->empty())
				varsMap.erase(i);
		}

		symbolStack.pop();
	}

	VarSymbolPtr getVar(const string &name) {
		if (!varsMap.count(name))
			return nullptr;
		return varsMap[name]->top();
	}

	FuncSymbolPtr getFunc(const string &name) {
		if (!funcsMap.count(name))
			return nullptr;
		return funcsMap[name];
	}

	void clear() {
		while (!symbolStack.empty())
			symbolStack.pop();

		varsMap.clear();
		funcsMap.clear();
	}

};

extern Symbol symbol;
extern vector<pair<Exception, int>> errorList;
