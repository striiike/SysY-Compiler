//
// Created by hiccup on 2023/11/14.
//

#pragma once

#include <string>
#include <vector>
#include <sstream>
#include "MipsInst.h"
using namespace std;

struct MipsGlobalVar {
	string name{};
	MipsLabel *label;

	vector<int> arr{};
	int size = 0;
	string stringLiteral{};

	string toString() const;
};

struct MipsBlock {
	vector<MipsInst *> instructionList{};
	string label;

	string toString() const;

	MipsBlock() = default;
};

struct MipsFunction {
	vector<MipsBlock *> blockList{};
	string name;

	string toString() const;

	MipsFunction() = default;
};

struct MipsModule {
	vector<MipsGlobalVar *> globals{};
	vector<MipsFunction *> functions{};

	MipsModule() {};

	void print(ostream &out);

};

