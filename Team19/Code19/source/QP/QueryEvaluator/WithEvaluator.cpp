#include "WithEvaluator.h"

/**
* Evaluate with clauses that have no common synonym with other clauses.
*
* @param withCl With clause to be evaluated
* @param facade Storage to be accessed
* @return boolean result if values exist for evaluated with clause
*/
bool WithEvaluator::evaluateNoCommSyn(shared_ptr<WithClause> withCl, GetFacade* facade)
{
	Argument arg1 = withCl->arg1;
	Argument arg2 = withCl->arg2;
	ArgsType type = QueryUtil::getArgumentsTypesFromMap(arg1, arg2);

	if (type == ArgsType::INTEGER_INTEGER)
	{
		// eg. with 1 = 1
		ArgString arg1Value = std::get<Integer>(arg1.argumentVariant).value;
		ArgString arg2Value = std::get<Integer>(arg2.argumentVariant).value;
		return arg1Value == arg2Value;
	}
	else if (type == ArgsType::QUOTE_QUOTE)
	{
		// with "proc_name_1" = "proc_name_1"
		ArgString arg1Value = std::get<StringExp>(arg1.argumentVariant).value;
		ArgString arg2Value = std::get<StringExp>(arg2.argumentVariant).value;
		return arg1Value == arg2Value;
	}
	else
	{
		Table resultTable = evaluateWithClause(withCl, facade);
		if (resultTable.getTableRows().size() == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	
}


/**
* Evaluate with clauses that have common synonym with other clauses, creating a table for comparison.
*
* @param withCl With clause to be evaluated
* @param facade Storage to be accessed
* @return table of synonym values that satisfy the with clause
*/
Table WithEvaluator::evaluateHasCommSyn(shared_ptr<WithClause> withCl, GetFacade* facade)
{
	Table resultTable = evaluateWithClause(withCl, facade);
	return resultTable;

}

/**
* Evaluate with clauses based on its arguments, creating a table for comparison.
*
* @param withCl With clause to be evaluated
* @param facade Storage to be accessed
* @return table of synonym values that satisfy the with clause
*/
Table WithEvaluator::evaluateWithClause(shared_ptr<WithClause> withCl, GetFacade* facade)
{
	Table resultTable;
	Argument arg1 = withCl->arg1;
	Argument arg2 = withCl->arg2;
	ArgsType argsType = QueryUtil::getArgumentsTypesFromMap(arg1, arg2);

	if (argsType == ArgsType::REFINT_REFINT)
	{
		// eg. statement s; constant c; with s.stmtno = c.value
		resultTable = evaluateWith_SynRefInt_SynRefInt(withCl, facade);
	}
	else if (argsType == ArgsType::REFINT_INTEGER)
	{
		// eg. statement p; with p.stmtno = 1
		resultTable = evaluateWith_SynRefInt_Int(withCl, facade);
	}
	else if (argsType == ArgsType::INTEGER_REFINT)
	{
		// eg. statement p; with 1 = p.stmtno
		resultTable = evaluateWith_Integer_SynRefInt(withCl, facade);
	}
	else if (argsType == ArgsType::REFNAME_REFNAME)
	{
		// eg. procedure p; call c; with p.procName = c.procName
		resultTable = evaluateWith_SynRefName_SynRefName(withCl, facade);
	}
	else if (argsType == ArgsType::REFNAME_QUOTE)
	{
		// eg. procedure p; with  p.procName = "proc_name_1"
		resultTable = evaluateWith_SynRefName_Quote(withCl, facade);
	}
	else if (argsType == ArgsType::QUOTE_REFNAME)
	{
		// eg. procedure p; with "proc_name_1" = p.procName
		resultTable = evaluateWith_Quote_SynRefName(withCl, facade);
	}
	
	return resultTable;
}

Table WithEvaluator::evaluateWith_SynRefInt_SynRefInt(shared_ptr<WithClause> withCl, GetFacade* facade)
{
	Table resultTable;

	Argument arg1 = withCl->arg1;
	Argument arg2 = withCl->arg2;
	Declaration arg1Dec = std::get<SynAttr>(arg1.argumentVariant).synonym;
	Declaration arg2Dec = std::get<SynAttr>(arg2.argumentVariant).synonym;
	SynonymResults values1 = arg1Dec.synType->getSynonymFromPKB(facade);
	SynonymResults values2 = arg2Dec.synType->getSynonymFromPKB(facade);

	resultTable = QueryUtil::createTableOnEqualValues(arg1Dec.synName, arg2Dec.synName, values1, values2);

	return resultTable;
}


Table WithEvaluator::evaluateWith_SynRefInt_Int(shared_ptr<WithClause> withCl, GetFacade* facade)
{
	Table resultTable;

	// 1. Get all stmtnos / constant values
	Argument arg1 = withCl->arg1;
	Declaration arg1Dec = std::get<SynAttr>(arg1.argumentVariant).synonym;
	SynonymResults values1 = arg1Dec.synType->getSynonymFromPKB(facade);
	// 2. Value to filter
	Argument arg2 = withCl->arg2;
	ArgString value = std::get<Integer>(arg2.argumentVariant).value;

	// Filter table of stmtnos
	resultTable = QueryUtil::createFilteredTableWithValue(arg1Dec.synName, values1, value);

	return resultTable;
}

Table WithEvaluator::evaluateWith_SynRefName_SynRefName(shared_ptr<WithClause> withCl, GetFacade* facade)
{
	Table resultTable;

	Argument arg1 = withCl->arg1;
	Argument arg2 = withCl->arg2;
	Declaration arg1Dec = std::get<SynAttr>(arg1.argumentVariant).synonym;
	Declaration arg2Dec = std::get<SynAttr>(arg2.argumentVariant).synonym;

	// 1. Get Table of synonym sttribute |p|p.procNamec.procName| (label as arg1+arg2 (pc), since we need to get intersection)
	TableItems items1 = arg1Dec.synType->getSynonymWithAttrRefFromPKB(facade, arg1Dec.synName, arg1Dec.synName + arg2Dec.synName);
	Table table1 = Table({ arg1Dec.synName, arg1Dec.synName + arg2Dec.synName }, items1);

	// 2. Get Table of synonym sttribute |c|p.procNamec.procName| (label as arg1+arg2, since we need to get intersection)
	TableItems items2 = arg2Dec.synType->getSynonymWithAttrRefFromPKB(facade, arg2Dec.synName, arg1Dec.synName + arg2Dec.synName);
	Table table2 = Table({ arg2Dec.synName, arg1Dec.synName + arg2Dec.synName }, items2);

	// 3. Join tables to get result table of |p|c|p.procNamec.procName|
	Table joinedTable = QueryUtil::joinTable(table1, table2);

	// 4. Remove items and header with arg1+arg2
	vector<string> headers = joinedTable.getTableHeader();
	auto it = find(headers.begin(), headers.end(), arg1Dec.synName + arg2Dec.synName);
	int index = it - headers.begin();
	headers.erase(headers.begin() + index);
	vector<vector<string>> items = joinedTable.getTableRows();
	for (auto& item : items)
	{
		item.erase(item.begin() + index);
	}
	resultTable = Table(headers, items);

	return resultTable;
}

Table WithEvaluator::evaluateWith_SynRefName_Quote(shared_ptr<WithClause> withCl, GetFacade* facade)
{
	Table resultTable;

	Argument arg1 = withCl->arg1;
	Declaration arg1Dec = std::get<SynAttr>(arg1.argumentVariant).synonym;

	Argument arg2 = withCl->arg2;
	ArgString value = std::get<StringExp>(arg2.argumentVariant).value;

	// 1. Get Table of synonym sttribute |p|pproc_name_1| (label as arg1+value, to prevent dup name)
	TableItems items1 = arg1Dec.synType->getSynonymWithAttrRefFromPKB(facade, arg1Dec.synName, arg1Dec.synName + value);
	Table table1 = Table({ arg1Dec.synName, arg1Dec.synName + value }, items1);

	// 2. Create table for value with header arg1+value |pproc_name_1| 
	Table table2 = Table({ arg1Dec.synName + value }, { {value} });

	// 3. Join tables to get result table of |p|pproc_name_1|
	Table joinedTable = QueryUtil::joinTable(table1, table2);

	// 4. Remove items and header with arg1+value
	vector<string> headers = joinedTable.getTableHeader();
	auto it = find(headers.begin(), headers.end(), arg1Dec.synName + value);
	int index = it - headers.begin();
	headers.erase(headers.begin() + index);
	vector<vector<string>> items = joinedTable.getTableRows();
	for (auto& item : items)
	{
		item.erase(item.begin() + index);
	}
	resultTable = Table(headers, items);

	return resultTable;
}

Table WithEvaluator::evaluateWith_Integer_SynRefInt(shared_ptr<WithClause> withCl, GetFacade* facade)
{
	Table resultTable;
	Argument arg2 = withCl->arg2;
	Declaration arg2Dec = std::get<SynAttr>(arg2.argumentVariant).synonym;
	SynonymResults values2 = arg2Dec.synType.get()->getSynonymFromPKB(facade);

	Argument arg1 = withCl->arg1;
	ArgString value = std::get<Integer>(arg1.argumentVariant).value;

	resultTable = QueryUtil::createFilteredTableWithValue(arg2Dec.synName, values2, value);

	return resultTable;
}

Table WithEvaluator::evaluateWith_Quote_SynRefName(shared_ptr<WithClause> withCl, GetFacade* facade)
{
	Table resultTable;

	Argument arg2 = withCl->arg2;
	Declaration arg2Dec = std::get<SynAttr>(arg2.argumentVariant).synonym;

	Argument arg1 = withCl->arg1;
	ArgString value = std::get<StringExp>(arg1.argumentVariant).value;

	// 1. Get Table of synonym sttribute |p|pproc_name_1| (label as arg1+value, to prevent dup name)
	TableItems items1 = arg2Dec.synType->getSynonymWithAttrRefFromPKB(facade, arg2Dec.synName, arg2Dec.synName + value);
	Table table1 = Table({ arg2Dec.synName, arg2Dec.synName + value }, items1);

	// 2. Create table for value with header arg1+value |pproc_name_1| 
	Table table2 = Table({ arg2Dec.synName + value }, { {value} });

	// 3. Join tables to get result table of |p|pproc_name_1|
	Table joinedTable = QueryUtil::joinTable(table1, table2);

	// 4. Remove items and header with arg1+value
	vector<string> headers = joinedTable.getTableHeader();
	auto it = find(headers.begin(), headers.end(), arg2Dec.synName + value);
	int index = it - headers.begin();
	headers.erase(headers.begin() + index);
	vector<vector<string>> items = joinedTable.getTableRows();
	for (auto& item : items)
	{
		item.erase(item.begin() + index);
	}
	resultTable = Table(headers, items);

	return resultTable;
}
