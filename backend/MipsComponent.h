//
// Created by hiccup on 2023/11/14.
//

#pragma once

#include <string>
#include <utility>
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

	string toString() const {
		if (!stringLiteral.empty()) {
			return name + ": .asciiz \"" + stringLiteral + "\"";
		}
		else if (arr.empty()) {
			return name + ": .space " + to_string(size);
		} else {
			stringstream ss;
			ss << name + ": .word ";
//			for (int i = (int)arr.size() - 1; i >= 0; --i) {
			for (int i : arr) {
				ss << to_string(i);
				ss << ", ";
			}
			return ss.str();
		}

	}
};

struct MipsBlock {
	vector<MipsInst *> instructionList{};
	string label;

	explicit MipsBlock(std::string name) : label(std::move(name)) {}

	void addInst(MipsInst *inst) {
		instructionList.push_back(inst);
	}

	string toString() const {
		stringstream ss;
		ss << label + ": \n";
		for (auto i : instructionList) {
			ss << "\t" + i->toString() + "\n";
		}
		return ss.str();
	}

};

struct MipsFunction {
	vector<MipsBlock *> blockList{};
	string name;
	int off = 0;

	explicit MipsFunction(std::string name) : name(std::move(name)) {}

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

	MipsModule() = default;

	void print(ostream &out) {
		out << ".macro getint\n"
			   "\tli\t$v0,\t5\n"
			   "\tsyscall\n"
			   ".end_macro\n\n"
			   ".macro putint\n"
			   "\tli\t$v0,\t1\n"
			   "\tsyscall\n"
			   ".end_macro\n\n"
			   ".macro putstr\n"
			   "\tli\t$v0,\t4\n"
			   "\tsyscall\n"
			   ".end_macro\n\n";


		out << ".data\n";
		for (auto i : globals) {
			out << i->toString() + "\n";
		}
		out << "\n.text\n";
		out << "\tjal\tFunction_main\n"
			   "\tli\t$v0,\t10\n"
			   "\tsyscall\n";
		for (auto i : functions) {
			out << i->toString() + "\n";
		}
	}

};

