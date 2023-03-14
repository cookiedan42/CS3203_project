#include "ResultConsolidator.h"

// Returns list<string> result for selecting boolean based on evaluation result
std::list<string> ResultConsolidator::selectBoolean(bool evalResult) 
{
	if (evalResult)
	{
		return { "TRUE" };
	}
	else
	{
		return { "FALSE" };
	}
}


// Returns list<string> result for selecting tuples (includes single and multiple type)
std::list<string> ResultConsolidator::selectTuple(SelectObject selectObj, Table table)
{
	std::list<std::string> results = {};

	// check if table empty
	if (table.getTableRows().size() == 0) {
		return results;
	}
	else {
		std::unordered_set<std::string> resultSet;
		std::vector<pair<Declaration, ArgType>> selectDecTypePairs = selectObj.selectPairs;
		std::vector<std::string> headers = table.getTableHeader();

		// get indexes of select declaration results
		std::vector<int> selectIdxs;
		for (auto& selectDecTypePair : selectDecTypePairs)
		{
			Declaration selectDec = selectDecTypePair.first;
			ArgType selectDecType = selectDecTypePair.second;

			// get index of declaration name / declaration attribute name in headers
			vector<string>::iterator it;
			if (selectDecType == ArgType::SYN) {
				it = find(headers.begin(), headers.end(), selectDec.synName);
			}
			else if (selectDecType == ArgType::ATTR_REF_INT) {
				it = find(headers.begin(), headers.end(), selectDec.synName + REF_INT_STR);
			}
			else if (selectDecType == ArgType::ATTR_REF_NAME) {
				it = find(headers.begin(), headers.end(), selectDec.synName + REF_INT_NAME);
			}

			// declaration name exists in headers
			if (it != headers.end())
			{
				int index = it - headers.begin();
				selectIdxs.push_back(index);
			}
			else {
				return results;
			}
		}

		// check if index list size is correct
		if (selectIdxs.size() < selectDecTypePairs.size())
		{
			return results;
		}

		// get tuple values
		for (auto& item : table.getTableRows())
		{
			std::string tuple = "";
			for (auto& idx : selectIdxs)
			{
				tuple += (item[idx] + " ");
			}
			tuple.pop_back();
			resultSet.insert(tuple);
		}

		return QueryUtil::set_to_list(resultSet);
	}
}


