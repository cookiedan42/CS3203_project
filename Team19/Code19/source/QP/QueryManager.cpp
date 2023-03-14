#include "QueryManager.h"
#include "QueryParser\QueryParser.h"
#include "QueryEvaluator/QueryEvaluator.h"
#include "QueryExceptions/QuerySyntaxException.h"
#include "QueryExceptions/QuerySemanticException.h"
#include "QuerySplitter/QuerySplitter.h"
#include "../Optimizer/Optimizer.h"
#include <list>
#include <string>

using namespace std;

list<string> QueryManager::getQueryResponse(string input, GetFacade* facade)
{
	try {
		QuerySplitter::validateHasSelect(input);
		auto tokens = QuerySplitter::tokenize(input);
		QuerySplitter querySplitter = QuerySplitter(tokens);
		querySplitter.parse();
		QueryParserResult result = QueryParser(querySplitter.getDeclarations(),
			querySplitter.getSelectArg(),
			querySplitter.getSuchThatClauses(),
			querySplitter.getPatternClauses(),
			querySplitter.getWithClauses())
			.parseQuery();

		QueryObject optimizedQueryObj = Optimizer::optimize(result);

		list<string> queryResult = QueryEvaluator::evaluate(optimizedQueryObj, facade);
		return queryResult;
	}
	catch (QuerySyntaxException synE) {
		return { "SyntaxError" };
	}
	catch (QuerySemanticException semE) {
		return { "SemanticError" };
	}
}
