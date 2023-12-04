//
// Created by hiccup on 2023/11/9.
//

#include <algorithm>
#include "DFBuilder.h"

#include "../llvm-ir/Function.h"
#include "../llvm-ir/instruction/BrInst.h"

DFBuilder::DFBuilder(list<Function *> *functions) {
	this->functions = functions;
}

void DFBuilder::run() {

	for (auto i : *functions) {
		preMap = new unordered_map<BasicBlock *, vector<BasicBlock *> *>();
		sucMap = new unordered_map<BasicBlock *, vector<BasicBlock *> *>();
		for (auto j : i->basicList) {
			(*preMap)[j] = new vector<BasicBlock *>();
			(*sucMap)[j] = new vector<BasicBlock *>();
			(*dominateMap)[j] = new vector<BasicBlock *>();
			(*dominateTree)[j] = new vector<BasicBlock *>();
			(*dominateFrontier)[j] = new vector<BasicBlock *>();
		}

		removeDeadBasicBlock(i);
		buildCFG(i);
		buildDominateMap(i);
		buildDominateTree(i);
		buildDF(i);

		// for GCM 
		buildDTDepth(i->basicList.front(), 1);
	}

}

void DFBuilder::buildCFG(Function *function) {
	preMap = new unordered_map<BasicBlock *, vector<BasicBlock *> *>();
	sucMap = new unordered_map<BasicBlock *, vector<BasicBlock *> *>();
	auto *BBs = new set<BasicBlock *>();

	for (auto i : function->basicList) {
		(*preMap)[i] = new vector<BasicBlock *>();
		(*sucMap)[i] = new vector<BasicBlock *>();
		BBs->insert(i);
	}

	/// make CFG map
	for (auto i : function->basicList) {
		auto *inst = i->getEndInst();
		if (inst->instType==BRANCH) {
			auto brInst = (BrInst *)inst;
			if (brInst->jump) {
				auto *tar = (BasicBlock *)brInst->getOp(0);
				(*sucMap)[i]->push_back(tar);
				(*preMap)[tar]->push_back(i);
			} else {
				auto *tar1 = (BasicBlock *)brInst->getOp(1);
				auto *tar2 = (BasicBlock *)brInst->getOp(2);
				(*sucMap)[i]->push_back(tar1);
				(*sucMap)[i]->push_back(tar2);
				(*preMap)[tar1]->push_back(i);
				(*preMap)[tar2]->push_back(i);
			}
		}
	}

	for (auto i : function->basicList) {
		i->preBbs = ((*preMap)[i]);
		i->sucBbs = ((*sucMap)[i]);
	}

	function->preMap = preMap;
	function->sucMap = sucMap;
}

void DFBuilder::removeDeadBasicBlock(Function *function) {
	for (auto &pair : *preMap)
		delete pair.second;
	preMap->clear();
	for (auto &pair : *sucMap)
		delete pair.second;
	sucMap->clear();

	std::set<BasicBlock *> BBs;

	for (auto i : function->basicList) {
		(*preMap)[i] = new vector<BasicBlock *>();
		(*sucMap)[i] = new vector<BasicBlock *>();

		auto &list = i->instructionList;
		auto it = std::find_if(list.begin(), list.end(),
							   [](auto &inst) {
								   return inst->instType==BRANCH || inst->instType==RETURN;
							   });

		if (it!=list.end()) {
			++it;  // reserve the branch inst
			list.erase(it, list.end());
		}
	}

	/// make CFG map
	for (auto i : function->basicList) {
		auto *inst = i->getEndInst();
		if (inst->instType==BRANCH) {
			auto brInst = (BrInst *)inst;
			if (brInst->jump) {
				auto *tar = (BasicBlock *)brInst->getOp(0);
				(*sucMap)[i]->push_back(tar);
				(*preMap)[tar]->push_back(i);
			} else {
				auto *tar1 = (BasicBlock *)brInst->getOp(1);
				auto *tar2 = (BasicBlock *)brInst->getOp(2);
				(*sucMap)[i]->push_back(tar1);
				(*sucMap)[i]->push_back(tar2);
				(*preMap)[tar1]->push_back(i);
				(*preMap)[tar2]->push_back(i);
			}
		}
	}

	auto *notDead = new set<BasicBlock *>();
	dfsNotDead(function->basicList.front(), notDead);
	for (auto it = function->basicList.begin(); it!=function->basicList.end();) {
		if (!notDead->count(*it)) {
			/// clear all @use relationship
			for (auto inst : (*it)->instructionList) {
				inst->removeUse();
			}
			(*it)->instructionList.clear();
			delete *it;
			it = function->basicList.erase(it);
		} else {
			++it;
		}
	}
}

void DFBuilder::dfsNotDead(BasicBlock *bb, set<BasicBlock *> *notDead) {
	if (notDead->find(bb)!=notDead->end()) {
		return;
	}
	notDead->insert(bb);
	for (auto next : *(sucMap->find(bb)->second)) {
		dfsNotDead(next, notDead);
	}
}

void DFBuilder::buildDominateMap(Function *function) {
	auto *entry = function->basicList.front();
	for (auto i : function->basicList) {
		auto *bbs = new set<BasicBlock *>;
		/// basicBlock can be reached from @entry without passing @i
		/// can't be dominated by @i
		dfsBlockedBy(i, entry, bbs);
		auto *children = new vector<BasicBlock *>;
		for (auto bb : function->basicList) {
			if (!bbs->count(bb))
				children->push_back(bb);
		}
		(*dominateMap)[i] = children;
	}
}

void DFBuilder::dfsBlockedBy(BasicBlock *tar, BasicBlock *entry,
							 set<BasicBlock *> *bbs) {
	if (tar==entry)
		return;
	bbs->insert(entry);
	/// entry's successors
	for (auto i : *(entry->sucBbs)) {
		if (!bbs->count(i)) {
			dfsBlockedBy(tar, i, bbs);
		}
	}
}

bool DFBuilder::isImmediatelyDominated(
	BasicBlock *dominator, BasicBlock *child) const {
	if (dominator==child)
		return false;
	for (auto middle : *((*dominateMap)[dominator])) {
		auto bbs = *((*dominateMap)[middle]);
		if (middle!=dominator && middle!=child &&
			std::count(bbs.begin(), bbs.end(), child)) {
			return false;
		}
	}
	return true;
}

void DFBuilder::buildDominateTree(Function *function) {
	for (auto dominator : function->basicList) {
		for (auto child : *((*dominateMap)[dominator])) {
			if (isImmediatelyDominated(dominator, child)) {
				(*dominateTree)[dominator]->push_back(child);
				child->immDominator = dominator;
			}
		}
	}

	function->dominateTree = dominateTree;

	for (auto i : function->basicList) {
		i->childBbs = (*dominateTree)[i];
	}
}

/// copy algorithm in https://buaa-se-compiling.github.io/miniSysY-tutorial/challenge/mem2reg/help.html
void DFBuilder::buildDF(Function *function) {
	for (auto a : *sucMap) {
		for (auto b : *(a.second)) {

			auto x = a.first;
			// while x does not strictly dominate b
			while (!(x!=b && std::count((*dominateMap)[x]->begin(), (*dominateMap)[x]->end(), b))) {
				(*dominateFrontier)[x]->push_back(b);
				x = x->immDominator;
			}
		}
	}

	for (auto bb : function->basicList) {
		bb->DF = (*dominateFrontier)[bb];
	}

}



void DFBuilder::buildDTDepth(BasicBlock *bb, int depth) {
	bb->dtDepth = depth;
	for (auto child : *bb->childBbs) {
		child->immDominator = bb;
		buildDTDepth(child, depth + 1);
	}
}

