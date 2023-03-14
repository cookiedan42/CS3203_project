#include "./Optimizer.h"

#include "../QP/QueryObject/DesignAbstractionFactory.h"

QueryObject Optimizer::optimize(QueryParserResult result) {
	result = rewriteSuchThat(result);
	QueryObject grouped = group(result);
	return sort(grouped);
}

//Groups the clauses based on the 3 different types defined in the QueryObject constructor
//Within each Group is guranteed to have a some shared synonym.
//Across each Group, its guranteed the synonyms have no relation.
QueryObject Optimizer::group(QueryParserResult result) {
	std::vector<ClauseGroup> mergedGroups = mergeGroups(result.getClausesWithAtLeastOneSynonym());
	return separateRelatedToSelectGroups(
		result.getSelectObj(), result.getDeclarations(), result.getNoSynClauses(), mergedGroups);
}

//sorts every group in the QO
QueryObject Optimizer::sort(QueryObject qo) {
	qo.clausesWithNoSynonym.sort();

	for (auto& group : qo.clauseGroupsRelatedToSelect) {
		group.sort();
	}
	for (auto& group : qo.clauseGroupsNotRelatedToSelect) {
		group.sort();
	}
	return qo;
}


std::vector<ClauseGroup> Optimizer::mergeGroups(std::deque<ClauseGroup> clausesWithAtLeastOneSynonym) {
	std::vector<ClauseGroup> mergedGroups;

	if (clausesWithAtLeastOneSynonym.size() == 0) return mergedGroups;

	//Keep iterating all clauses belong to a group.
	while (!clausesWithAtLeastOneSynonym.empty()) {
		ClauseGroup currentGroup = clausesWithAtLeastOneSynonym.front();
		clausesWithAtLeastOneSynonym.pop_front();

		size_t prevSize = clausesWithAtLeastOneSynonym.size();
		bool isMergable = true;

		//While clauses can still be merged in the current group, continue checking across all the remaining clauses
		//One cycle of remaining clauses must be checked if some merging was done.
		while (isMergable) {
			merge(currentGroup, clausesWithAtLeastOneSynonym, prevSize);

			//meaning after checking all remaining groups, no change in queue size, means no more to merge for the current Group.
			if (clausesWithAtLeastOneSynonym.size() == prevSize) {
				isMergable = false;
				mergedGroups.push_back(currentGroup);
			}
			else {
				//else if there is a change in #remaining groups, means some group was merged before and there is a possibility of merging again.
				prevSize = clausesWithAtLeastOneSynonym.size();
			}
		}
	}
	return mergedGroups;
}

void Optimizer::merge(ClauseGroup& currentGroup, std::deque<ClauseGroup>& clauseGroups, size_t& groupSize) {
	//runs for 1 iteration of remaining groups
	for (size_t i = 0; i < groupSize; ++i) {
		ClauseGroup nextGroup = clauseGroups.front();
		clauseGroups.pop_front();

		//Merge if there exists some common synonym
		if (currentGroup.hasCommSyn(nextGroup.getGroupSynNames())) {
			currentGroup.merge(nextGroup);
		}
		else {
			clauseGroups.push_back(nextGroup);
		}
	}
}

QueryObject Optimizer::separateRelatedToSelectGroups(SelectObject selectObj,
	std::vector<Declaration> declarations, ClauseGroup clausesWithNoSynonym, std::vector<ClauseGroup> mergedGroups) {
	std::vector<ClauseGroup> clauseGroupsRelatedToSelect;
	std::vector<ClauseGroup> clauseGroupsNotRelatedToSelect;
	std::unordered_set<std::string> selectSyns;

	//could be improved on selectObj side, but it works for now
	for (auto& decTypePair : selectObj.selectPairs) {
		Declaration d = decTypePair.first;
		selectSyns.emplace(d.synName);
	}

	for (auto& group : mergedGroups) {
		if (group.hasCommSyn(selectSyns)) {
			clauseGroupsRelatedToSelect.push_back(group);
		}
		else {
			clauseGroupsNotRelatedToSelect.push_back(group);
		}
	}

	return QueryObject(selectObj, declarations, clausesWithNoSynonym,
		clauseGroupsNotRelatedToSelect,
		clauseGroupsRelatedToSelect
	);

}


/* 
Rewrites QueryParserResult
shifts some from hasSyn to hasnoSyn
no change to select object or to declarations
	- equivalent to stmt s1,s2,s3; Select s1;
*/
QueryParserResult Optimizer::rewriteSuchThat(const QueryParserResult& result) {
	auto& selectObject = result.getSelectObj();
	auto& clauseGroupsWithSyn = result.getClausesWithAtLeastOneSynonym();
	ClauseGroup newNoSynClauses = result.getNoSynClauses();
	std::deque<ClauseGroup> newClauseGroupsWithSyn{};

	auto synUsedCount = getSynUsedCount(selectObject, clauseGroupsWithSyn);
	
	// copy syn clauses and modify as needed
	for (const auto& clgrp : clauseGroupsWithSyn) {
		// clause groups in here are ClauseGroups of size 1 because they are not yet grouped
		const shared_ptr<Clause> cl_uncast = *(clgrp.getRelatedClauses().cbegin());
		auto cl = dynamic_pointer_cast<SuchThatClause>(cl_uncast);
		if (!cl) {
			newClauseGroupsWithSyn.push_back(clgrp);
			continue;
		}

		// only modify Affects, Affects* and Next*
		// which are the three evaluators where algo different based on args
		const auto& relType =cl->relationship->type;
		if (relType != AFFECTS_REL && relType != AFFECTS_T_REL && relType != NEXT_T_REL) {
			newClauseGroupsWithSyn.push_back(clgrp);
			continue;
		}

		auto modifiedClause = modifyRelevantClause(cl, synUsedCount);
		 
		if (modifiedClause->getSynonyms().size() == 0) {
			newNoSynClauses.addClauseToGroup(modifiedClause);
		} else {
			ClauseGroup newGroup = ClauseGroup();
			newGroup.addClauseToGroup(modifiedClause);
			newClauseGroupsWithSyn.push_back(newGroup);
		}
	}
	return QueryParserResult(result.getDeclarations(), selectObject, newNoSynClauses, newClauseGroupsWithSyn);
}

SynCountMap Optimizer::getSynUsedCount(const SelectObject& selectObject, const std::deque<ClauseGroup> clauseGroups) {
	SynCountMap synUsedCount{};

	// add syn used by select
	for (auto& [decl, argtype] : selectObject.selectPairs) {
		if (!synUsedCount.count(decl.synName)) synUsedCount.insert({ decl.synName, 1 });
	}

	// add syn used by clauses
	for (const auto& clgrp : clauseGroups) {
		// clause groups in here are ClauseGroups of size 1 because they are not yet grouped
		const shared_ptr<Clause> cl = *(clgrp.getRelatedClauses().cbegin());
		for (auto& i : cl->getSynonyms()) {
			if (!synUsedCount.count(i.synName)) synUsedCount.insert({ i.synName, 0 });

			synUsedCount.at(i.synName)++;
		}
	}

	return synUsedCount;
}

shared_ptr<Clause> Optimizer::modifyRelevantClause(const shared_ptr<SuchThatClause>& clause, const SynCountMap& synUsedCount) {
	const auto& relType = clause->relationship->type;
	Argument arg1 = clause->relationship->arg1;
	Declaration* dec1 = std::get_if<Declaration>(&arg1.argumentVariant);
	Argument arg2 = clause->relationship->arg2;
	Declaration* dec2 = std::get_if<Declaration>(&arg2.argumentVariant);

	// replace assign in affects
	if (relType == AFFECTS_REL || relType == AFFECTS_T_REL) {
		if (shouldModify(dec1, ASSIGN_DE, synUsedCount)) arg1 = { WildCard() };
		if (shouldModify(dec2, ASSIGN_DE, synUsedCount)) arg2 = { WildCard() };
	}

	// replace stmt and assign for both
	if (shouldModify(dec1, STATEMENT_DE, synUsedCount)) arg1 = { WildCard() };
	if (shouldModify(dec2, STATEMENT_DE, synUsedCount)) arg2 = { WildCard() };

	auto newDA = DesignAbstractionFactory::createDesignAbstraction(relType, arg1, arg2);

	return make_shared<SuchThatClause>(newDA);
}

bool Optimizer::shouldModify(const Declaration* dec, const std::string& synType, const SynCountMap& synUsedCount) {
	return dec && dec->synType->type == synType && synUsedCount.at(dec->synName) == 1;
}
