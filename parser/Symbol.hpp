//
// Created by hiccup on 2023/10/1.
//

#pragma once

#include<bits/stdc++.h>

#include <utility>
#include "Exception.hpp"

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
        auto scope = *symbolStack.top();
        if (scope.count(name)) return 0;

        scope.insert(name);
        if (!varsMap.count(name)) varsMap[name] = make_shared<stack<VarSymbolPtr>>();
        bool isGlobal = symbolStack.size() == 1;
        varsMap[name]->push(make_shared<VarSymbol>(isConst, isGlobal, lens, values));
        return 1;
    }

    // 0 for false
    int insertFunc(const string &name, bool isVoid, const vector<int> &dims = {}) {
        if (funcsMap.count(name)) return 0;

        funcsMap.emplace(name, make_shared<FuncSymbol>(isVoid, dims));
        return 1;
    }

    void startScope() {
        symbolStack.push(make_shared<set<string>>());
    }

    void endScope() {
        SymbolSetPtr set = symbolStack.top();
        for (const auto &i: *set) {
            varsMap[i]->pop();
            if (varsMap[i]->empty()) varsMap.erase(i);
        }
    }

    VarSymbolPtr getVar(const string &name) {
        if (!varsMap.count(name)) return nullptr;
        return varsMap[name]->top();
    }

    FuncSymbolPtr getFunc(const string &name) {
        if (!funcsMap.count(name)) return nullptr;
        return funcsMap[name];
    }
};

extern Symbol symbol;
extern vector<pair<Exception, int>> errorList;
