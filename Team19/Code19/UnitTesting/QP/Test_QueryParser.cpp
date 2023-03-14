#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/QP/QuerySplitter/QuerySplitter.h"
#include "../source/QP/QueryParser/QueryParser.h"
#include "../source/QP/QueryObject/Declaration.h"
#include "../source/QP/QueryObject/QueryObject.h"
#include "../source/QP/QueryObject/SelectObject.h"
#include "../source/QP/QueryObject/DesignAbstractionFactory.h"
#include "../source/QP/QueryExceptions/QuerySyntaxException.h"
#include "../source/QP/QueryExceptions/QuerySemanticException.h"
#include <codecvt>
#include <locale>
#include <vector>
#include <unordered_map>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestQueryParser) {
public:
	/* NO COND CLAUSES (such-that, pattern, with) */
	TEST_METHOD(Test_invalidSelectUseUndeclaredSynThrowException) {
		string query = "assign a; Select b";
		QuerySplitter::validateHasSelect(query);
		auto tokens = QuerySplitter::tokenize(query);
		auto qs = QuerySplitter(tokens);
		qs.parse();

		Assert::ExpectException<QuerySemanticException>([&]() {
			QueryParserResult qObj = QueryParser(qs.getDeclarations(),
				qs.getSelectArg(), qs.getSuchThatClauses(), qs.getPatternClauses(), qs.getWithClauses())
				.parseQuery();
			});
	}

	TEST_METHOD(Test_invalidSelectUseMissingSynThrowException) {
		string query = "assign a; Select";
		Assert::ExpectException<QuerySyntaxException>([&]() {
			QuerySplitter::validateHasSelect(query);
			auto tokens = QuerySplitter::tokenize(query);
			auto qs = QuerySplitter(tokens);
			qs.parse();
			QueryParserResult qObj = QueryParser(qs.getDeclarations(), qs.getSelectArg(), qs.getSuchThatClauses(), qs.getPatternClauses(), qs.getWithClauses())
				.parseQuery();
			});
	}

	TEST_METHOD(Test_invalidSelectSmallSyntaxUseThrowException) {
		string query = "assign a; select a";
		Assert::ExpectException<QuerySyntaxException>([&]() {
			QuerySplitter::validateHasSelect(query);
			auto tokens = QuerySplitter::tokenize(query);
			auto qs = QuerySplitter(tokens);
			qs.parse();
			QueryParserResult qObj = QueryParser(qs.getDeclarations(), qs.getSelectArg(), qs.getSuchThatClauses(), qs.getPatternClauses(), qs.getWithClauses())
				.parseQuery();
			});
	}

	TEST_METHOD(Test_invalidDesignEntityRefUseThrowException) {
		string query = "assign a; var v; Select v";
		Assert::ExpectException<QuerySyntaxException>([&]() {
			QuerySplitter::validateHasSelect(query);
			auto tokens = QuerySplitter::tokenize(query);
			auto qs = QuerySplitter(tokens);
			qs.parse();
			QueryParserResult qObj = QueryParser(qs.getDeclarations(), qs.getSelectArg(), qs.getSuchThatClauses(), qs.getPatternClauses(), qs.getWithClauses())
				.parseQuery();
			});
	}

	TEST_METHOD(Test_missingSelectStatementThrowException) {
		string query = "variable v; stmt s";
		Assert::ExpectException<QuerySyntaxException>([&]() {
			QuerySplitter::validateHasSelect(query);
			auto tokens = QuerySplitter::tokenize(query);
			auto qs = QuerySplitter(tokens);
			qs.parse();
			QueryParserResult qObj = QueryParser(qs.getDeclarations(), qs.getSelectArg(), qs.getSuchThatClauses(), qs.getPatternClauses(), qs.getWithClauses())
				.parseQuery();
			});
	}

	TEST_METHOD(Test_missingDeclarationsThrowException) {
		string query = "Select s";
		Assert::ExpectException<QuerySyntaxException>([&]() {
			QuerySplitter::validateHasSelect(query);
			auto tokens = QuerySplitter::tokenize(query);
			auto qs = QuerySplitter(tokens);
			qs.parse();
			QueryParserResult qObj = QueryParser(qs.getDeclarations(), qs.getSelectArg(), qs.getSuchThatClauses(), qs.getPatternClauses(), qs.getWithClauses())
				.parseQuery();
			});
	}

	TEST_METHOD(Test_basicProcedure) {
		string query = "procedure p; Select p";
		QuerySplitter::validateHasSelect(query);
		auto tokens = QuerySplitter::tokenize(query);
		auto qs = QuerySplitter(tokens);
		qs.parse();

		std::vector<Declaration> expectedDecs = {
			Declaration(make_shared<DE::Procedure>(), "p"),
		};
		SelectObject expectedSelectObj = SelectObject({ make_pair(expectedDecs[0], ArgType::SYN) });
		QueryParserResult expectedQObj = QueryParserResult(expectedDecs, expectedSelectObj,
			{}, {}, {}, {}, {}, {}
		);

		QueryParserResult qObj = QueryParser(qs.getDeclarations(),
			qs.getSelectArg(), qs.getSuchThatClauses(), qs.getPatternClauses(), qs.getWithClauses())
			.parseQuery();
		Assert::IsTrue(expectedQObj == qObj);
	}

	TEST_METHOD(Test_basicDeclarations) {
		string query = "assign a; variable v; Select a"; 
		QuerySplitter::validateHasSelect(query);
		auto tokens = QuerySplitter::tokenize(query);
		auto qs = QuerySplitter(tokens);
		qs.parse();

		std::vector<Declaration> expectedDecs = {
			Declaration(make_shared<DE::Assign>(), "a"),
			Declaration(make_shared<DE::Variable>(), "v")
		};
		SelectObject expectedSelectObj = SelectObject({ make_pair(expectedDecs[0], ArgType::SYN) });
		QueryParserResult expectedQObj = QueryParserResult(expectedDecs, expectedSelectObj, {}, {}, {}, {}, {}, {});

		QueryParserResult qObj = QueryParser(qs.getDeclarations(),
			qs.getSelectArg(), qs.getSuchThatClauses(), qs.getPatternClauses(), qs.getWithClauses())
			.parseQuery();
		Assert::IsTrue(expectedQObj == qObj);
	}

	/* SUCH-THAT CLAUSES */

	TEST_METHOD(Test_SuchThatWhileIntParent) {
		string query = "while w; Select w such that Parent(w,10)";
		QuerySplitter::validateHasSelect(query);
		auto tokens = QuerySplitter::tokenize(query);
		auto qs = QuerySplitter(tokens);
		qs.parse();

		Declaration whileDec = Declaration(make_shared<DE::While>(), "w");
		std::vector<Declaration> expectedDecs = { whileDec };
		struct Argument arg1 = { whileDec };
		struct Argument arg2 = { StmtNo("10") };
		std::vector<SuchThatClause> expectedStCl = { SuchThatClause(DesignAbstractionFactory::createDesignAbstraction("Parent", arg1, arg2)) };
		SelectObject expectedSelectObj = SelectObject({ make_pair(expectedDecs[0], ArgType::SYN) });
		QueryParserResult expectedQObj = QueryParserResult(expectedDecs, expectedSelectObj, {}, expectedStCl, {}, {}, {}, {});

		QueryParserResult qObj = QueryParser(qs.getDeclarations(), qs.getSelectArg(), qs.getSuchThatClauses(), qs.getPatternClauses(), qs.getWithClauses())
			.parseQuery();
		Assert::IsTrue(expectedQObj == qObj);
	}
	TEST_METHOD(Test_SuchThatStmtIntNextStar) {
		string query = "stmt s; Select s such that Next*(s,10)";
		QuerySplitter::validateHasSelect(query);
		auto tokens = QuerySplitter::tokenize(query);
		auto qs = QuerySplitter(tokens);
		qs.parse();
		QueryParserResult qObj = QueryParser(qs.getDeclarations(), qs.getSelectArg(), qs.getSuchThatClauses(), qs.getPatternClauses(), qs.getWithClauses())
			.parseQuery();
		Declaration stmtDec = Declaration(make_shared<DE::Statement>(), "s");
		std::vector<Declaration> expectedDecs = { stmtDec };
		struct Argument arg1 = { stmtDec };
		struct Argument arg2 = { StmtNo("10") };
		std::vector<SuchThatClause> expectedStCl = { SuchThatClause(DesignAbstractionFactory::createDesignAbstraction("Next*", arg1, arg2)) };
		SelectObject expectedSelectObj = SelectObject({ make_pair(expectedDecs[0], ArgType::SYN) });
		QueryParserResult expectedQObj = QueryParserResult(expectedDecs, expectedSelectObj, {}, expectedStCl, {}, {}, {}, {});

		Assert::IsTrue(expectedQObj == qObj);
	}

	TEST_METHOD(Test_SuchThatAffectsStar) {
		string query = "assign a; Select a such that Affects* (a, 13)";
		QuerySplitter::validateHasSelect(query);
		auto tokens = QuerySplitter::tokenize(query);
		auto qs = QuerySplitter(tokens);
		qs.parse();
		QueryParserResult qObj = QueryParser(qs.getDeclarations(), qs.getSelectArg(), qs.getSuchThatClauses(), qs.getPatternClauses(), qs.getWithClauses())
			.parseQuery();
		Declaration stmtDec = Declaration(make_shared<DE::Assign>(), "a");
		std::vector<Declaration> expectedDecs = { stmtDec };
		struct Argument arg1 = { stmtDec };
		struct Argument arg2 = { StmtNo("13") };
		std::vector<SuchThatClause> expectedStCl = { SuchThatClause(DesignAbstractionFactory::createDesignAbstraction("Affects*", arg1, arg2)) };
		SelectObject expectedSelectObj = SelectObject({ make_pair(expectedDecs[0], ArgType::SYN) });
		QueryParserResult expectedQObj = QueryParserResult(expectedDecs, expectedSelectObj, {}, expectedStCl, {}, {}, {}, {});

		Assert::IsTrue(expectedQObj == qObj);
	}

	/* SINGLE-PATTERN CLAUSES */
	TEST_METHOD(Test_PatternWildcardWildcardParse) {
		string query = "assign a; Select a pattern a(_, _)";
		QuerySplitter::validateHasSelect(query);
		auto tokens = QuerySplitter::tokenize(query);
		auto qs = QuerySplitter(tokens);
		qs.parse();

		Declaration keyDec = Declaration(make_shared<DE::Assign>(), "a");
		std::vector<Declaration> expectedDecs = { keyDec };
		SelectObject expectedSelectObj = SelectObject({
			make_pair(keyDec, ArgType::SYN)
			});
		struct Argument arg1 = { WildCard() };
		struct Argument arg2 = { WildCard() };
		std::vector<PatternClause> expectedPtCl = { PatternClause(keyDec, arg1, arg2) };
		QueryParserResult expectedQObj = QueryParserResult(expectedDecs, expectedSelectObj, {}, {}, {}, expectedPtCl, {}, {});

		QueryParserResult qObj = QueryParser(qs.getDeclarations(), qs.getSelectArg(), qs.getSuchThatClauses(), qs.getPatternClauses(), qs.getWithClauses()).parseQuery();
		Assert::IsTrue(expectedQObj == qObj);
	}

	TEST_METHOD(Test_PatternSynonymWildcardParse) {
		string query = "assign a; variable v; Select a pattern a(v, _)";
		QuerySplitter::validateHasSelect(query);
		auto tokens = QuerySplitter::tokenize(query);
		auto qs = QuerySplitter(tokens);
		qs.parse();

	Declaration assignDec = Declaration(make_shared<DE::Assign>(), "a");
	Declaration variableDec = Declaration(make_shared<DE::Variable>(), "v");
	std::vector<Declaration> expectedDecs = { assignDec, variableDec };
	struct Argument arg1 = { variableDec };
	struct Argument arg2 = { WildCard() };
	std::vector<PatternClause> expectedPtCl = { PatternClause(assignDec, arg1, arg2) };
	SelectObject expectedSelectObj = SelectObject({
		make_pair(assignDec, ArgType::SYN)
		});
	QueryParserResult expectedQObj = QueryParserResult(expectedDecs, expectedSelectObj, {}, {}, {}, expectedPtCl, {}, {});

		QueryParserResult qObj = QueryParser(qs.getDeclarations(), qs.getSelectArg(), qs.getSuchThatClauses(), qs.getPatternClauses(), qs.getWithClauses()).parseQuery();
		Assert::IsTrue(expectedQObj == qObj);
	}

	TEST_METHOD(Test_PatternStringExpWildcardParse) {
		string query = "assign a; Select a pattern a(\"x\", _)";
		QuerySplitter::validateHasSelect(query);
		auto tokens = QuerySplitter::tokenize(query);
		auto qs = QuerySplitter(tokens);
		qs.parse();

		Declaration assignDec = Declaration(make_shared<DE::Assign>(), "a");
		std::vector<Declaration> expectedDecs = { assignDec };
		SelectObject expectedSelectObj = SelectObject({
			make_pair(assignDec, ArgType::SYN)
			});
		struct Argument arg1 = { StringExp("x") };
		struct Argument arg2 = { WildCard() };
		std::vector<PatternClause> expectedPtCl = { PatternClause(assignDec, arg1, arg2) };
		QueryParserResult expectedQObj = QueryParserResult(expectedDecs, expectedSelectObj, {}, {}, {}, expectedPtCl, {}, {});

		QueryParserResult qObj = QueryParser(qs.getDeclarations(), qs.getSelectArg(), qs.getSuchThatClauses(), qs.getPatternClauses(), qs.getWithClauses()).parseQuery();
		Assert::IsTrue(expectedQObj == qObj);
	}

	TEST_METHOD(Test_PatternWildcardEquationParse) {
		string query = "assign a; Select a pattern a(_, \"(x + y) * z / 1 - x % y\")";
		QuerySplitter::validateHasSelect(query);
		auto tokens = QuerySplitter::tokenize(query);
		auto qs = QuerySplitter(tokens);
		qs.parse();

		Declaration keyDec = Declaration(make_shared<DE::Assign>(), "a");
		std::vector<Declaration> expectedDecs = { keyDec };
		SelectObject expectedSelectObj = SelectObject({
			make_pair(keyDec, ArgType::SYN)
			});
		struct Argument arg1 = { WildCard() };
		struct Argument arg2 = { StringExp("(x+y)*z/1-x%y") };
		std::vector<PatternClause> expectedPtCl = { PatternClause(keyDec, arg1, arg2) };
		QueryParserResult expectedQObj = QueryParserResult(expectedDecs, expectedSelectObj, {}, {}, {}, expectedPtCl, {}, {});

		QueryParserResult qObj = QueryParser(qs.getDeclarations(), qs.getSelectArg(), qs.getSuchThatClauses(), qs.getPatternClauses(), qs.getWithClauses()).parseQuery();
		Assert::IsTrue(expectedQObj == qObj);
	}

	TEST_METHOD(Test_PatternVarEquationParse) {
		string query = "assign a; variable v; Select a pattern a(v, \"(x + y)\")";
		QuerySplitter::validateHasSelect(query);
		auto tokens = QuerySplitter::tokenize(query);
		auto qs = QuerySplitter(tokens);
		qs.parse();

		Declaration assignDec = Declaration(make_shared<DE::Assign>(), "a");
		Declaration varDec = Declaration(make_shared<DE::Variable>(), "v");
		std::vector<Declaration> expectedDecs = { assignDec, varDec };
		SelectObject expectedSelectObj = SelectObject({
			make_pair(assignDec, ArgType::SYN)
			});
		struct Argument arg1 = { varDec };
		struct Argument arg2 = { StringExp("(x+y)") };
		std::vector<PatternClause> expectedPtCl = { PatternClause(assignDec, arg1, arg2) };
		QueryParserResult expectedQObj = QueryParserResult(expectedDecs, expectedSelectObj, {}, {}, {}, expectedPtCl, {}, {});

		QueryParserResult qObj = QueryParser(qs.getDeclarations(), qs.getSelectArg(), qs.getSuchThatClauses(), qs.getPatternClauses(), qs.getWithClauses()).parseQuery();
		Assert::IsTrue(expectedQObj == qObj);
	}

	TEST_METHOD(Test_PatternStringEquationParse) {
		string query = "assign a; Select a pattern a(\"z\", \"(x + y)\")";
		QuerySplitter::validateHasSelect(query);
		auto tokens = QuerySplitter::tokenize(query);
		auto qs = QuerySplitter(tokens);
		qs.parse();

		Declaration assignDec = Declaration(make_shared<DE::Assign>(), "a");
		std::vector<Declaration> expectedDecs = { assignDec };
		SelectObject expectedSelectObj = SelectObject({
			make_pair(assignDec, ArgType::SYN)
			});
		struct Argument arg1 = { StringExp("z") };
		struct Argument arg2 = { StringExp("(x+y)") };
		std::vector<PatternClause> expectedPtCl = { PatternClause(assignDec, arg1, arg2) };
		QueryParserResult expectedQObj = QueryParserResult(expectedDecs, expectedSelectObj, {}, {}, {}, expectedPtCl, {}, {});

		QueryParserResult qObj = QueryParser(qs.getDeclarations(), qs.getSelectArg(), qs.getSuchThatClauses(), qs.getPatternClauses(), qs.getWithClauses()).parseQuery();
		Assert::IsTrue(expectedQObj == qObj);
	}

	TEST_METHOD(Test_PatternWildcardSubExpEquationParse) {
		string query = "assign a; Select a pattern a(_, _\"(x + y) * z / 1 - x % y\"_)";
		QuerySplitter::validateHasSelect(query);
		auto tokens = QuerySplitter::tokenize(query);
		auto qs = QuerySplitter(tokens);
		qs.parse();

		Declaration keyDec = Declaration(make_shared<DE::Assign>(), "a");
		std::vector<Declaration> expectedDecs = { keyDec };
		SelectObject expectedSelectObj = SelectObject({
			make_pair(keyDec, ArgType::SYN)
			});
		struct Argument arg1 = { WildCard() };
		struct Argument arg2 = { SubExp("(x+y)*z/1-x%y") };
		std::vector<PatternClause> expectedPtCl = { PatternClause(keyDec, arg1, arg2) };
		QueryParserResult expectedQObj = QueryParserResult(expectedDecs, expectedSelectObj, {}, {}, {}, expectedPtCl, {}, {});

		QueryParserResult qObj = QueryParser(qs.getDeclarations(), qs.getSelectArg(), qs.getSuchThatClauses(), qs.getPatternClauses(), qs.getWithClauses()).parseQuery();
		Assert::IsTrue(expectedQObj == qObj);
	}

	TEST_METHOD(Test_PatternVarBracketedEquationParse) {
		string query = "assign a; variable v; Select a pattern a(v, _\"(x + y)\"_)";
		QuerySplitter::validateHasSelect(query);
		auto tokens = QuerySplitter::tokenize(query);
		auto qs = QuerySplitter(tokens);
		qs.parse();

		Declaration assignDec = Declaration(make_shared<DE::Assign>(), "a");
		Declaration varDec = Declaration(make_shared<DE::Variable>(), "v");
		std::vector<Declaration> expectedDecs = { assignDec, varDec };
		SelectObject expectedSelectObj = SelectObject({
			make_pair(assignDec, ArgType::SYN)
			});
		struct Argument arg1 = { varDec };
		struct Argument arg2 = { SubExp("(x+y)") };
		std::vector<PatternClause> expectedPtCl = { PatternClause(assignDec, arg1, arg2) };
		QueryParserResult expectedQObj = QueryParserResult(expectedDecs, expectedSelectObj, {}, {}, {}, expectedPtCl, {}, {});

		QueryParserResult qObj = QueryParser(qs.getDeclarations(), qs.getSelectArg(), qs.getSuchThatClauses(), qs.getPatternClauses(), qs.getWithClauses()).parseQuery();
		Assert::IsTrue(expectedQObj == qObj);
	}

	TEST_METHOD(Test_PatternVarConstEquationParse) {
		string query = "assign a; variable v; Select a pattern a(v, _\"1\"_)";
		QuerySplitter::validateHasSelect(query);
		auto tokens = QuerySplitter::tokenize(query);
		auto qs = QuerySplitter(tokens);
		qs.parse();

		Declaration assignDec = Declaration(make_shared<DE::Assign>(), "a");
		Declaration varDec = Declaration(make_shared<DE::Variable>(), "v");
		std::vector<Declaration> expectedDecs = { assignDec, varDec };
		SelectObject expectedSelectObj = SelectObject({
			make_pair(assignDec, ArgType::SYN)
			});
		struct Argument arg1 = { varDec };
		struct Argument arg2 = { SubExp("1") };
		std::vector<PatternClause> expectedPtCl = { PatternClause(assignDec, arg1, arg2) };
		QueryParserResult expectedQObj = QueryParserResult(expectedDecs, expectedSelectObj, {}, {}, {}, expectedPtCl, {}, {});

		QueryParserResult qObj = QueryParser(qs.getDeclarations(), qs.getSelectArg(), qs.getSuchThatClauses(), qs.getPatternClauses(), qs.getWithClauses()).parseQuery();
		Assert::IsTrue(expectedQObj == qObj);
	}

	TEST_METHOD(Test_PatternVarVarEquationParse) {
		string query = "assign a; variable v; Select a pattern a(v, _\"x123\"_)";
		QuerySplitter::validateHasSelect(query);
		auto tokens = QuerySplitter::tokenize(query);
		auto qs = QuerySplitter(tokens);
		qs.parse();

		Declaration assignDec = Declaration(make_shared<DE::Assign>(), "a");
		Declaration varDec = Declaration(make_shared<DE::Variable>(), "v");
		std::vector<Declaration> expectedDecs = { assignDec, varDec };
		SelectObject expectedSelectObj = SelectObject({
			make_pair(assignDec, ArgType::SYN)
			});
		struct Argument arg1 = { varDec };
		struct Argument arg2 = { SubExp("x123") };
		std::vector<PatternClause> expectedPtCl = { PatternClause(assignDec, arg1, arg2) };
		QueryParserResult expectedQObj = QueryParserResult(expectedDecs, expectedSelectObj, {}, {}, {}, expectedPtCl, {}, {});

		QueryParserResult qObj = QueryParser(qs.getDeclarations(), qs.getSelectArg(), qs.getSuchThatClauses(), qs.getPatternClauses(), qs.getWithClauses()).parseQuery();
		Assert::IsTrue(expectedQObj == qObj);
	}

	/* WITH CLAUSES */
	TEST_METHOD(Test_withClauseHaveSynParse) {
		string query = "read r; Select r with r.varName = \"x\"";
		QuerySplitter::validateHasSelect(query);
		auto tokens = QuerySplitter::tokenize(query);
		auto qs = QuerySplitter(tokens);
		qs.parse();

		Declaration keyDec = Declaration(make_shared<DE::Read>(), "r");
		std::vector<Declaration> expectedDecs = { keyDec };
		SelectObject expectedSelectObj = SelectObject({
			make_pair(keyDec, ArgType::SYN)
			});
		struct SynAttr lhsSynAttr = { keyDec, ArgType::ATTR_REF_INT };
		struct Argument lhsArg = { lhsSynAttr };
		struct Argument rhsArg = { StringExp("x") };
		std::vector<WithClause> expectedWithCl = { WithClause(lhsArg, rhsArg) };
		QueryParserResult expectedQObj = QueryParserResult(expectedDecs, expectedSelectObj, {}, {}, {}, {}, {}, expectedWithCl);

		QueryParserResult qObj = QueryParser(qs.getDeclarations(), qs.getSelectArg(), qs.getSuchThatClauses(), qs.getPatternClauses(), qs.getWithClauses()).parseQuery();
		Assert::IsTrue(expectedQObj == qObj);
	}

	TEST_METHOD(Test_withClauseNoSynParse) {
		string query = "read r; Select r with 3 = 2";
		QuerySplitter::validateHasSelect(query);
		auto tokens = QuerySplitter::tokenize(query);
		auto qs = QuerySplitter(tokens);
		qs.parse();

		Declaration keyDec = Declaration(make_shared<DE::Read>(), "r");
		std::vector<Declaration> expectedDecs = { keyDec };
		SelectObject expectedSelectObj = SelectObject({
			make_pair(keyDec, ArgType::SYN)
			});
		struct Argument arg1 = { Integer("3") };
		struct Argument arg2 = { Integer("2") };
		std::vector<WithClause> expectedWithCl = { WithClause(arg1, arg2) };
		QueryParserResult expectedQObj = QueryParserResult(expectedDecs, expectedSelectObj, {}, {}, {}, {}, expectedWithCl, {});

		QueryParserResult qObj = QueryParser(qs.getDeclarations(), qs.getSelectArg(), qs.getSuchThatClauses(), qs.getPatternClauses(), qs.getWithClauses()).parseQuery();
		Assert::IsTrue(expectedQObj == qObj);
	}

	TEST_METHOD(Test_withClauseInvalidStringsNoDelimParse) {
		string query = "procedure p; variable v; Select p with string = string";
		Assert::ExpectException<QuerySyntaxException>([&]() {
			QuerySplitter::validateHasSelect(query);
			auto tokens = QuerySplitter::tokenize(query);
			auto qs = QuerySplitter(tokens);
			qs.parse();
			QueryParser(qs.getDeclarations(), qs.getSelectArg(), qs.getSuchThatClauses(), qs.getPatternClauses(), qs.getWithClauses()).parseQuery();
			});
	}

	TEST_METHOD(Test_withClauseInvalidStrParamsNoDelimParse) {
		string query = "procedure p; variable v; Select v with string = p";
		Assert::ExpectException<QuerySyntaxException>([&]() {
			QuerySplitter::validateHasSelect(query);
			auto tokens = QuerySplitter::tokenize(query);
			auto qs = QuerySplitter(tokens);
			qs.parse();
			QueryParser(qs.getDeclarations(), qs.getSelectArg(), qs.getSuchThatClauses(), qs.getPatternClauses(), qs.getWithClauses()).parseQuery();
			});
	}

	TEST_METHOD(Test_withClauseInvalidStrComparisonParamsNoDelimParse) {
		string query = "stmt s; Select s with s.stmt# = \"var\"";
		QuerySplitter::validateHasSelect(query);
		auto tokens = QuerySplitter::tokenize(query);
		auto qs = QuerySplitter(tokens);
		qs.parse();
		Assert::ExpectException<QuerySemanticException>([&]() {
			QueryParser(qs.getDeclarations(), qs.getSelectArg(), qs.getSuchThatClauses(), qs.getPatternClauses(), qs.getWithClauses()).parseQuery();
			});
	}

	TEST_METHOD(Test_withClauseInvalidIntAsStringParamsNoDelimParse) {
		string query = "stmt s; Select s with s.stmt# = \"3\"";
		Assert::ExpectException<QuerySyntaxException>([&]() {
			QuerySplitter::validateHasSelect(query);
			auto tokens = QuerySplitter::tokenize(query);
			auto qs = QuerySplitter(tokens);
			qs.parse();
			QueryParser(qs.getDeclarations(), qs.getSelectArg(), qs.getSuchThatClauses(), qs.getPatternClauses(), qs.getWithClauses()).parseQuery();
			});
	}

	TEST_METHOD(Test_withClauseInvalidStmtNumStringCompareNoDelimParse) {
		string query = "stmt s; Select s with s.stmt# = \"statementNum\"";
		Assert::ExpectException<QuerySemanticException>([&]() {
			QuerySplitter::validateHasSelect(query);
			auto tokens = QuerySplitter::tokenize(query);
			auto qs = QuerySplitter(tokens);
			qs.parse();
			QueryParser(qs.getDeclarations(), qs.getSelectArg(), qs.getSuchThatClauses(), qs.getPatternClauses(), qs.getWithClauses()).parseQuery();
			});
	}

	TEST_METHOD(Test_withClauseIntFirstParse) {
		string query = "assign a, a1; Select a1 with 15 = a.stmt#";
		QuerySplitter::validateHasSelect(query);
		auto tokens = QuerySplitter::tokenize(query);
		auto qs = QuerySplitter(tokens);
		qs.parse();

		Declaration assignDecOne = Declaration(make_shared<DE::Assign>(), "a");
		Declaration assignDecTwo = Declaration(make_shared<DE::Assign>(), "a1");
		std::vector<Declaration> expectedDecs = { assignDecOne, assignDecTwo };
		SelectObject expectedSelectObj = SelectObject({
			make_pair(assignDecTwo, ArgType::SYN)
			});
		struct Argument arg1 = { StmtNo("15") };
		struct Argument arg2 = { assignDecOne };
		std::vector<WithClause> expectedWithCl = { WithClause(arg1, arg2) };
		QueryParserResult expectedQObj = QueryParserResult(expectedDecs, expectedSelectObj, {}, {}, {}, {}, {}, expectedWithCl);

		QueryParserResult qObj = QueryParser(qs.getDeclarations(), qs.getSelectArg(), qs.getSuchThatClauses(), qs.getPatternClauses(), qs.getWithClauses()).parseQuery();
		Assert::IsTrue(expectedQObj == qObj);
	}

	/* Select Variations */
	/** BOOLEAN **/
	TEST_METHOD(Test_specialBooleanCaseParse) {
		string query = "variable BOOLEAN; Select BOOLEAN";
		QuerySplitter::validateHasSelect(query);
		auto tokens = QuerySplitter::tokenize(query);
		auto qs = QuerySplitter(tokens);
		qs.parse();

		Declaration keyDec = Declaration(make_shared<DE::Variable>(), "BOOLEAN");
		std::vector<Declaration> expectedDecs = { keyDec };
		SelectObject expectedSelectObj = SelectObject({
			make_pair(keyDec, ArgType::SYN)
			});
		QueryParserResult expectedQObj = QueryParserResult(expectedDecs, expectedSelectObj, {}, {}, {}, {}, {}, {});

		QueryParserResult qObj = QueryParser(qs.getDeclarations(), qs.getSelectArg(), qs.getSuchThatClauses(), qs.getPatternClauses(), qs.getWithClauses()).parseQuery();
		Assert::IsTrue(expectedQObj == qObj);
	}
	};
}