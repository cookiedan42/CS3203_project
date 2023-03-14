#include "QueryObject.h"
#include "SuchThatClause.h"
#include "PatternClause.h"
#include "Declaration.h"

QueryObject::QueryObject(SelectObject selectObj, std::vector<Declaration> declarations,
	ClauseGroup clausesWithNoSynonym,
	std::vector<ClauseGroup> clauseGroupsNotRelatedToSelect,
	std::vector<ClauseGroup> clauseGroupsRelatedToSelect)
	: selectObj(selectObj), declarations(declarations),
	clausesWithNoSynonym(clausesWithNoSynonym),
	clauseGroupsNotRelatedToSelect(clauseGroupsNotRelatedToSelect),
	clauseGroupsRelatedToSelect(clauseGroupsRelatedToSelect)
{

}

//bool QueryObject::operator==(const QueryObject& qo) const {
//	return tie(declarations, selectObj, clausesWithNoSynonym, clauseGroupsNotRelatedToSelect, clauseGroupsRelatedToSelect)
//		== tie(qo.declarations, qo.selectObj, qo.clausesWithNoSynonym, qo.clauseGroupsNotRelatedToSelect, qo.clauseGroupsRelatedToSelect);
//}