//
// Created by hiccup on 2023/11/23.
//

#include "MipsAllocator.h"
#include "mips/MipsComponent.h"
#include <algorithm>
#include <iostream>
#include <fstream>

void MipsAllocator::buildDefUse(MipsBlock *bb) {
	auto blockDef = new set<MipsReg *>();
	auto blockUse = new set<MipsReg *>();
	for (auto i : bb->instList) {
		/*
		 *  def before use count as blockDef
		 *  use before def count as blockUse
		 *  when use & def happens the same time, it counts use
		 */
		for (auto j : *i->getUse()) {
			if (!blockDef->count(j))
				blockUse->insert(j);
		}

		for (auto j : *i->getDef()) {
			if (!blockUse->count(j))
				blockDef->insert(j);
		}
	}
	bb->def = blockDef;
	bb->use = blockUse;
}

void MipsAllocator::liveAnalysis(MipsFunction *f) {
	for (auto bb : f->blockList) {
		buildDefUse(bb);
	}

	bool changed = true;
	while (changed) {
		changed = false;

		/*
		 * in[n] = use[n] + (out[n] - def[n])
		 * out[n] += for succ in[n]
		 *
		 */
		cout << "fucker\n" << endl;
		for (auto bb : f->blockList) {
			auto *_liveOut = new set<MipsReg *>;
			auto *_liveIn = new set<MipsReg *>;

			std::set<MipsReg *> difference{};
			std::set_difference(
				bb->liveOut->begin(), bb->liveOut->end(),
				bb->def->begin(), bb->def->end(),
				std::inserter(difference, difference.begin()));

			_liveIn->insert(bb->use->begin(), bb->use->end());
			_liveIn->insert(difference.begin(), difference.end());

			for (auto suc : *bb->succ) {
				_liveOut->insert(suc->liveIn->begin(), suc->liveIn->end());
			}

			if (*_liveIn!=*bb->liveIn || *_liveOut!=*bb->liveOut) {
				changed = true;
			}

			bb->liveIn = _liveIn;
			bb->liveOut = _liveOut;
		}
	}

	for (auto bb : f->blockList) {
		cout << bb->label << ": " << endl;

		cout << "live in :\n";
		for (auto i : *bb->liveIn) {
			cout << i->toString() << ", ";
		}
		cout << endl;
		cout << "live out :\n";
		for (auto i : *bb->liveIn) {
			cout << i->toString() << ", ";
		}
		cout << endl;
	}
}

void MipsAllocator::clearState() {
	simplifyWorklist.clear();
	freezeWorklist.clear();
	spillWorklist.clear();
	spilledNodes.clear();
	coalescedNodes.clear();
	coloredNodes.clear();
	selectStack = stack<MipsReg *>{};
	selectSet.clear();

	/*
	 *  move instruction
	 */
	coalescedMoves.clear();
	constrainedMoves.clear();
	frozenMoves.clear();
	worklistMoves.clear();
	activeMoves.clear();

	initial.clear();
	adjSet.clear();
	degree.clear();
	alias.clear();
	color.clear();

	for (auto i : adjList) {
		i.second.clear();
	}
	for (auto i : moveList) {
		i.second.clear();
	}
	adjList.clear();
	moveList.clear();
}

void MipsAllocator::run() {

	for (auto f : module->functions) {

		clearState();

		restart:
		for (auto bb : f->blockList) {
			for (auto i : bb->instList) {
				initial.insert(i->getDef()->begin(), i->getDef()->end());
				initial.insert(i->getUse()->begin(), i->getUse()->end());
			}
		}

		liveAnalysis(f);
		build(f);
		makeWorkList();

		while (!simplifyWorklist.empty() || !worklistMoves.empty() ||
			!freezeWorklist.empty() || !spillWorklist.empty()) {
			if (!simplifyWorklist.empty()) {
				simplify();
			}
			if (!worklistMoves.empty()) {
				coalesce();
			}
			if (!freezeWorklist.empty()) {
				freeze();
			}
			if (!spillWorklist.empty()) {
				selectSpill();
			}
		}

		assignColors();

		cout << "spilledNodes : "<< endl;
		for (auto i : spilledNodes) {
			cout << i->toString() << " ";
		}
		cout << endl;

		/*
		cout << "colors : "<< endl;
		for (auto i : color) {
			cout << i.first->toString() << " " << to_string(i.second) << ", ";
		}
		cout << endl;
		 */

		if (!spilledNodes.empty()) {
			rewriteProgram(f);
			goto restart;
		}

		/*
		 *  TODO: replace reg with physics color
		 *
		 *  TODO: temporarily using t-series reg, not considering like $ra $v0
		 */
		for (auto vr : color) {
			((MipsVrReg *)vr.first)->alloca = index2reg[vr.second + 8];
			f->allocatedRegs.insert(index2reg[vr.second + 8]);
		}
		f->allocatedRegs.insert($ra);
//		f->allocatedRegs.insert($v0);
	}



	std::ofstream tmp("./mips_withoutFix.txt");
	module->print(tmp);


	/*
	 *  TODO: fix stack, spilled nodes allocation
	 */
	for (auto f : module->functions) {

		// make register to vector
		auto vec = vector<MipsReg*>(f->allocatedRegs.begin(), f->allocatedRegs.end());
		int allocateSize = f->stackOff + 4*(int)vec.size();

		// allocate size for spill nodes


		auto b1 = new MipsBlock(f->name + "_save_context");
		f->blockList.push_front(b1);
		for (int i = 0; i < vec.size(); ++i) {
			auto reg = vec[i];
			auto s = new MipsStoreInst(reg, new MipsImm(-i*4-4), $sp);
			b1->addInst(s);
		}

		auto add = new MipsBinInst(M_ADDU, $sp, $sp, new MipsImm(-allocateSize));
		b1->addInst(add);

		for (auto bb : f->blockList) {
			for (auto i : bb->instList) {
				if (dynamic_cast<MipsLoadInst *>(i) &&
					((MipsLoadInst *)i)->offset==$sp &&
					!i->allocateForSpill) {
					dynamic_cast<MipsImm *>(((MipsLoadInst *)i)->addr)->imm += allocateSize;
				}
			}
		}

		/*
		 *  have to insert before any jr, fuck!
		 */
		auto comment = new MipsComment(f->name + "_load_context");

		auto b2 = new MipsBlock(f->name + "_load_context");

		auto sub = new MipsBinInst(M_ADDU, $sp, $sp, new MipsImm(allocateSize));
		b2->addInst(sub);

		for (int i = 0; i < vec.size(); ++i) {
			auto reg = vec[i];
			auto s = new MipsLoadInst(reg, new MipsImm(-i*4-4), $sp);
			b2->addInst(s);
		}

		for (auto bb : f->blockList) {
			for (auto i : bb->instList) {

				if (dynamic_cast<MipsBranchInst*>(i) && ((MipsBranchInst*)i)->ty == CondType::JR) {

					for (auto j : b2->instList) {
						bb->insertBefore(i, j);
					}

				}
			}
		}

	}
}

void MipsAllocator::build(MipsFunction *f) {
	for (auto bb : f->blockList) {

		auto *live = bb->liveOut;
		for (int i = (int)bb->instList.size() - 1; i >= 0; i--) {
			auto inst = bb->instList[i];

			if (dynamic_cast<MipsLiInst *>(inst) &&
				dynamic_cast<MipsVrReg *>(((MipsLiInst *)inst)->dst) &&
				dynamic_cast<MipsVrReg *>(((MipsLiInst *)inst)->imm)) {
				for (auto use : *inst->getUse()) {
					live->erase(use);
					moveList[use].insert((MipsLiInst *)inst);
				}

				for (auto def : *inst->getDef()) {
					moveList[def].insert((MipsLiInst *)inst);
				}

				worklistMoves.insert((MipsLiInst *)inst);
			}
			live->insert(inst->getDef()->begin(), inst->getDef()->end());

			for (auto d : *inst->getDef()) {
				for (auto l : *live) {
					addEdge(l, d);
				}
			}

			for (auto def : *inst->getDef()) {
				live->erase(def);
			}
			live->insert(inst->getUse()->begin(), inst->getUse()->end());
		}
	}
}

void MipsAllocator::addEdge(MipsReg *u, MipsReg *v) {

	if (!adjSet.count(make_pair(u, v)) && u!=v) {
		adjSet.insert(make_pair(u, v));
		adjSet.insert(make_pair(v, u));
		if (!u->isPreColored()) {
			adjList[u].insert(v);
			degree[u] += 1;
		}
		if (!v->isPreColored()) {
			adjList[v].insert(u);
			degree[v] += 1;
		}
	}
}

void MipsAllocator::makeWorkList() {
	for (auto reg : initial) {
		if (degree[reg] >= K) {
			spillWorklist.insert(reg);
		} else if (false) {
		} else if (moveRelated(reg)) {
			freezeWorklist.insert(reg);
		} else {
			simplifyWorklist.insert(reg);
		}
	}
	initial.clear();
}

unordered_set<MipsReg *> MipsAllocator::adjacent(MipsReg *n) {
	auto ret = unordered_set<MipsReg *>{adjList[n]};
	for (auto i : coalescedNodes) {
		ret.erase(i);
	}
	for (auto i : selectSet) {
		ret.erase(i);
	}
	return ret;
}

unordered_set<MipsLiInst *> MipsAllocator::nodeMoves(MipsReg *n) {
	auto ret = unordered_set<MipsLiInst *>{};
	for (auto move : moveList[n]) {
		if (activeMoves.count(move) || worklistMoves.count(move)) {
			ret.insert(move);
		}
	}
	return ret;
}

bool MipsAllocator::moveRelated(MipsReg *n) {
	return !nodeMoves(n).empty();
}

void MipsAllocator::simplify() {
	auto n = *(simplifyWorklist.begin());
	simplifyWorklist.erase(n);

	selectStack.push(n);
	selectSet.insert(n);
	for (auto m : adjacent(n)) {
		decrementDegree(m);
	}
}

void MipsAllocator::decrementDegree(MipsReg *m) {
	auto d = degree[m];
	degree[m] = d - 1;
	if (d==K) {
		auto nodes = unordered_set<MipsReg *>{adjacent(m)};
		nodes.insert(m);
		enableMoves(nodes);
		spillWorklist.erase(m);
		if (moveRelated(m)) {
			freezeWorklist.insert(m);
		} else {
			simplifyWorklist.insert(m);
		}
	}
}

void MipsAllocator::enableMoves(unordered_set<MipsReg *> &nodes) {
	for (auto n : nodes) {
		for (auto m : nodeMoves(n)) {
			if (activeMoves.count(m)) {
				activeMoves.erase(m);
				worklistMoves.insert(m);
			}
		}
	}
}

void MipsAllocator::coalesce() {
	auto m = *worklistMoves.begin();
	auto x = getAlias(dynamic_cast<MipsReg *>(m->dst));
	auto y = getAlias(dynamic_cast<MipsReg *>(m->imm));
	MipsReg *u, *v;
	if (y->isPreColored()) {
		u = y;
		v = x;
	} else {
		u = x;
		v = y;
	}
	worklistMoves.erase(m);

	auto adjacentUV = unordered_set<MipsReg *>{adjacent(u)};
	adjacentUV.insert(adjacent(v).begin(), adjacent(v).end());

	if (u==v) {
		coalescedMoves.insert(m);
		addWorkList(u);
	} else if (v->isPreColored() || adjSet.count(make_pair(u, v))) {
		constrainedMoves.insert(m);
		addWorkList(u);
		addWorkList(v);
	} else if ((u->isPreColored() &&
		std::all_of(adjacent(v).begin(), adjacent(v).end(),
					[&](const auto &t) { return OK(t, u); })) ||
		(!u->isPreColored() && conservative(adjacentUV))) {

		coalescedMoves.insert(m);
		combine(u, v);
		addWorkList(u);
	} else {
		activeMoves.insert(m);
	}
}

void MipsAllocator::addWorkList(MipsReg *u) {
	if (!u->isPreColored() && !moveRelated(u) && degree[u] < K) {
		freezeWorklist.erase(u);
		simplifyWorklist.insert(u);
	}
}

bool MipsAllocator::OK(MipsReg *t, MipsReg *r) {
	return degree[t] < K || t->isPreColored() || adjSet.count(make_pair(t, r));
}

bool MipsAllocator::conservative(unordered_set<MipsReg *> &nodes) {
	auto k = 0;
	for (auto n : nodes) {
		if (degree[n] >= K) {
			k++;
		}
	}
	return k < K;
}

MipsReg *MipsAllocator::getAlias(MipsReg *n) {
	if (coalescedNodes.count(n)) {
		return getAlias(alias[n]);
	} else {
		return n;
	}
}

void MipsAllocator::combine(MipsReg *u, MipsReg *v) {
	if (freezeWorklist.count(v)) {
		freezeWorklist.erase(v);
	} else {
		spillWorklist.erase(v);
	}
	coalescedNodes.insert(v);
	alias[v] = u;
	moveList[u].insert(moveList[v].begin(), moveList[v].end());
	auto tmp = unordered_set<MipsReg *>{v};
	enableMoves(tmp);
	for (auto t : adjacent(v)) {
		addEdge(t, v);
		decrementDegree(t);
	}
	if (degree[u] >= K && freezeWorklist.count(u)) {
		freezeWorklist.erase(u);
		spillWorklist.insert(u);
	}
}

void MipsAllocator::freeze() {
	auto u = *freezeWorklist.begin();
	freezeWorklist.erase(u);
	simplifyWorklist.insert(u);
	freezeMoves(u);
}

void MipsAllocator::freezeMoves(MipsReg *u) {
	for (auto m : nodeMoves(u)) {
		auto x = (MipsReg *)m->dst, y = (MipsReg *)m->imm;
		MipsReg *v;
		if (getAlias(y)==getAlias(u)) {
			v = getAlias(x);
		} else {
			v = getAlias(y);
		}
		activeMoves.erase(m);
		frozenMoves.insert(m);
		if (nodeMoves(v).empty() && degree[v] < K) {
			freezeWorklist.erase(v);
			simplifyWorklist.insert(v);
		}
	}
}

/*
 *  再找找吧
 */
void MipsAllocator::selectSpill() {
	auto m = *(spillWorklist.rbegin());

	spillWorklist.erase(m);
	simplifyWorklist.insert(m);
	freezeMoves(m);
}

void MipsAllocator::assignColors() {
	while (!selectStack.empty()) {
		auto n = selectStack.top();
		selectStack.pop();
		selectSet.erase(n);

		auto okColors = set<int>{};
		for (int i = 0; i < K; i++) {
			okColors.insert(i);
		}

		for (auto w : adjList[n]) {
			if (getAlias(w)->isPreColored() || coloredNodes.count(getAlias(w))) {
				okColors.erase(color[getAlias(w)]);
			}
		}

		if (okColors.empty()) {
			spilledNodes.insert(n);
		} else {
			coloredNodes.insert(n);
			auto c = *okColors.begin();
			color[n] = c;
		}
	}

	for (auto n : coalescedNodes) {
		color[n] = color[getAlias(n)];
	}
}

void MipsAllocator::rewriteProgram(MipsFunction *f) {
	/*
	 *  我觉得应该够吧..., who knows it 2333, 有一个点不够，md
	 */




	auto newTemps = unordered_set<MipsReg *>{};

//	f->stackOff -= f->allocateForSpill;
	f->allocateForSpill = 0;

	for (auto spill : spilledNodes) {

		MipsReg *vrReg = nullptr;

		for (auto bb: f->blockList) {
			auto insts = bb->instList;
			for (auto i : insts) {

				if (!vrReg)
					vrReg = new MipsVrReg();

				// whether used or not
				bool _use = i->replaceUse2vr(spill, vrReg);
				bool _def = i->replaceDef2vr(spill, vrReg);

				if (_use) {
					auto m = new MipsLoadInst(vrReg, new MipsImm(f->stackOff), $sp);
					m->allocateForSpill = true;
					bb->insertBefore(i,m);
				}

				if (_def) {
					auto m = new MipsStoreInst(vrReg, new MipsImm(f->stackOff), $sp);
					m->allocateForSpill = true;
					bb->insertAfter(i,m);
				}

				if (_use || _def) {
					newTemps.insert(vrReg);
					vrReg = nullptr;
				}

			}
		}

		// allocate space on the stack
		f->stackOff += 4;

		// total allocate, if rewriteProgram again
		f->allocateForSpill += 4;

	}

	cout << "!Allocate stack size : "
		 << f->stackOff
		 << ", allocate for spill : "
		 << f->allocateForSpill << endl;

	spilledNodes.clear();
	initial.clear();
	initial.insert(coloredNodes.begin(), coloredNodes.end());
	initial.insert(coalescedNodes.begin(), coalescedNodes.end());
	initial.insert(newTemps.begin(), newTemps.end());
	coloredNodes.clear();
	coalescedNodes.clear();
}








