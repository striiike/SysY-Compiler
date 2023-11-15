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
	string stringLiteral{};

	string toString() const;
};

struct MipsBlock {
	vector<MipsInst *> instructionList{};
	string label;

	string toString() const {
		stringstream ss;
		ss << label + ": \n";
		for (auto i : instructionList) {
			ss << "\t" + i->toString() + "\n";
		}
		return ss.str();
	}

	MipsBlock() = default;
};

struct MipsFunction {
	vector<MipsBlock *> blockList{};
	string name;

	string toString() const {
		stringstream ss;
		ss << "Function_" + name + ": \n";
		for (auto i : blockList) {
			ss << "  " + i->toString();
		}
		return ss.str();
	}

	MipsFunction() = default;
};

struct MipsModule {
	vector<MipsGlobalVar *> globals{};
	vector<MipsFunction *> functions{};

	MipsModule() {};

	void print(ostream &out) {
		out << ".data\n";
		for (auto i : globals) {
			out << i->toString() + "\n";
		}
		out << ".text\n";
		for (auto i : functions) {
			out << i->toString() + "\n";
		}
	}

};

