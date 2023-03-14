#pragma once
#include "../QP/QueryParser/QueryParserResult.h"
#include "../QP/QueryObject/QueryObject.h"
#include "../QP/QueryObject/ClauseGroup.h"
#include "../TypeAlias/TypeAlias.h"
#include <queue>

class Optimizer {
public:
	static QueryObject optimize(QueryParserResult result);

	static QueryParserResult rewriteSuchThat(const QueryParserResult& result);

	static QueryObject group(QueryParserResult result);

	static QueryObject sort(QueryObject qo);

private:
	static std::vector<ClauseGroup> mergeGroups(std::deque<ClauseGroup> clausesWithAtLeastOneSynonym);

	static void merge(ClauseGroup& currentGroup, std::deque<ClauseGroup>& clauseGroups, size_t& groupSize);

	static QueryObject separateRelatedToSelectGroups(SelectObject selectObj,
		std::vector<Declaration> declarations, ClauseGroup clausesWithNoSynonym, std::vector<ClauseGroup> mergedGroups);

	static SynCountMap getSynUsedCount(const SelectObject& selectObject, const std::deque<ClauseGroup> clauseGroups);

	static shared_ptr<Clause> modifyRelevantClause(const shared_ptr<SuchThatClause>& clause, const SynCountMap& synUsedCount);

	static bool shouldModify(const Declaration* dec, const std::string& synType, const SynCountMap& synUsedCount);
};