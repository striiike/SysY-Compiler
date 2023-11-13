//
// Created by hiccup on 2023/11/9.
//

#ifndef COMPILER_MIDEND_DFBUILDER_H
#define COMPILER_MIDEND_DFBUILDER_H

#include <vector>
#include <unordered_map>
#include <set>
#include <list>

using namespace std;

class Function;
class BasicBlock;

class DFBuilder {
public:
	list<Function *> *functions{};
	unordered_map<BasicBlock *, vector<BasicBlock *> *>
		*preMap = new unordered_map<BasicBlock *, vector<BasicBlock *> *>();
	unordered_map<BasicBlock *, vector<BasicBlock *> *>
		*sucMap = new unordered_map<BasicBlock *, vector<BasicBlock *> *>();

	unordered_map<BasicBlock *, vector<BasicBlock *> *>
		*dominateMap = new unordered_map<BasicBlock *, vector<BasicBlock *> *>();

	unordered_map<BasicBlock *, vector<BasicBlock *> *>
		*dominateTree = new unordered_map<BasicBlock *, vector<BasicBlock *> *>();

	unordered_map<BasicBlock *, vector<BasicBlock *> *>
		*dominateFrontier = new unordered_map<BasicBlock *, vector<BasicBlock *> *>();

public:
	explicit DFBuilder(list<Function *> *functions);
	void run();
	void buildCFG(Function *function);

	void removeDeadBasicBlock(Function *function);
	void dfsNotDead(BasicBlock *bb, set<BasicBlock *> *notDead);
	void buildDominateMap(Function *function);
	void dfsBlockedBy(BasicBlock *tar, BasicBlock *entry, set<BasicBlock *> *bbs);
	void buildDominateTree(Function *function);
	bool isImmediatelyDominated(BasicBlock *dominator, BasicBlock *child) const;
	void buildDF(Function *function);
};

#endif //COMPILER_MIDEND_DFBUILDER_H
