//
// Created by hiccup on 2023/11/23.
//

#pragma once

#include "mips/MipsComponent.h"
#include <unordered_map>
#include <unordered_set>
#include <stack>

class MipsAllocator {
public:
	MipsModule *module;
	int K = 21;
	/*
	 *  stack, workList, set
	 */
	unordered_set<MipsReg *> simplifyWorklist;
	unordered_set<MipsReg *> freezeWorklist;
	set<MipsReg *> spillWorklist;
	unordered_set<MipsReg *> spilledNodes;
	unordered_set<MipsReg *> coalescedNodes;
	unordered_set<MipsReg *> coloredNodes;
	stack<MipsReg *> selectStack;
	unordered_set<MipsReg *> selectSet;

	/*
	 *  move instruction
	 */
	unordered_set<MipsLiInst *> coalescedMoves;
	unordered_set<MipsLiInst *> constrainedMoves;
	unordered_set<MipsLiInst *> frozenMoves;
	unordered_set<MipsLiInst *> worklistMoves;
	unordered_set<MipsLiInst *> activeMoves;

	/*
	 *  others
	 */
	unordered_set<MipsReg *> initial;
	set<pair<MipsReg *, MipsReg *>> adjSet;
	unordered_map<MipsReg *, unordered_set<MipsReg *>> adjList;
	unordered_map<MipsReg *, int> degree;
	unordered_map<MipsReg *, unordered_set<MipsLiInst *>> moveList;
	unordered_map<MipsReg *, MipsReg *> alias;
	unordered_map<MipsReg *, int> color;

public:
	explicit MipsAllocator(MipsModule *module) { this->module = module; }
	void buildDefUse(MipsBlock *bb);
	void liveAnalysis(MipsFunction *f);
	void run();
	void build(MipsFunction *f);
	void addEdge(MipsReg *u, MipsReg *v);
	void makeWorkList();
	unordered_set<MipsReg *> adjacent(MipsReg *n);
	unordered_set<MipsLiInst *> nodeMoves(MipsReg *n);
	bool moveRelated(MipsReg *n);
	void simplify();
	void decrementDegree(MipsReg *m);
	void enableMoves(unordered_set<MipsReg *> &nodes);
	void coalesce();
	MipsReg *getAlias(MipsReg *n);
	void addWorkList(MipsReg *u);
	bool OK(MipsReg *t, MipsReg *r);
	bool conservative(unordered_set<MipsReg *> &nodes);
	void combine(MipsReg *u, MipsReg *v);
	void freeze();
	void freezeMoves(MipsReg *u);
	void selectSpill();
	void assignColors();
	void rewriteProgram(MipsFunction *f);
	void clearState();
};

/*
 *  move 有 bug, 不过也优化了没多少，不管了。
 */