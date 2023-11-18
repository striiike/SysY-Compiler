//
// Created by hiccup on 2023/10/1.
//

#pragma once

#include "Exception.hpp"
#include <set>
#include <stack>
#include <map>
#include <iostream>

using namespace std;

class Value;

class VarSymbol {
public:
	bool isConst;
	bool isGlobal;
	vector<int> lens;  // at most 2 dimensions
	vector<int> values;     // save it in 1 dimension

	Value *llvmValue;

	VarSymbol(bool isConst, bool isGlobal,
			  vector<int> lens, vector<int> values,
			  Value *llvm = nullptr);

	// get value from indexes
	int getValue(const vector<int> &index);

	Value *getLlvmValue() const;

};

class FuncSymbol {
public:
	bool isVoid;
	vector<int> dims;       // stand for the types
	Value *llvmValue;

	FuncSymbol(bool isVoid, vector<int> dims, Value *llvmValue = nullptr);

	Value *getLlvmValue() const;

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
	int insertVar(bool isConst, const string &name,
				  const vector<int> &lens, const vector<int> &values = {},
				  Value *llvm = nullptr);

	// 0 for false
	int insertFunc(const string &name, bool isVoid, const vector<int> &dims = {}, Value *func = nullptr);

	void startScope();

	void endScope();

	VarSymbolPtr getVar(const string &name);

	FuncSymbolPtr getFunc(const string &name);

	void clear();

};

extern Symbol symbol;
extern vector<pair<Exception, int>> errorList;
