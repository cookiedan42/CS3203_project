#include "stdafx.h"
#include "CppUnitTest.h"
#include <codecvt>
#include <locale>
#include "../source/QP/QuerySplitter/QuerySplitter.h"

namespace Microsoft {
	namespace VisualStudio {
		namespace CppUnitTestFramework {
			template<>
			static std::wstring ToString<std::vector<pair<std::string, std::string>>>(const std::vector<pair<std::string, std::string>>& decList) {
				std::string result = "Declaration pairs:\n";
				for (auto itr = decList.begin(); itr != decList.end(); ++itr) {
					result += "synName-'" + itr->first + "' type-'" + itr->second + "'\n";
				}
				return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(result);
			}
			
			template<>
			static std::wstring ToString<vector<Token>>(const vector<Token>& selectStmt) {
				std::string result = "SelectArg with clauses: \n";
				for (Token token : selectStmt) {
					result += "'" + token.getValue() + "'";
				}
				result += "\n";
				return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(result);
			}
			
			template<>
			static std::wstring ToString<vector<vector<Token>>>(const vector<vector<Token>>& clauseList) {
				std::string result = "ClauseList with clauses: \n";
				for (vector<Token> clause : clauseList) {
					for (Token token : clause) {
						result += token.getValue();
					}
					result += "\n";
				}
				return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(result);
			}
		}
	}
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestQuerySplitter) {

	using clauses_t = std::vector< std::vector<Token>>;
	using declarations_t = std::unordered_map<std::string, std::string>;

	struct testhelper {

		QuerySplitter makeQS(std::string inQuery) {
			auto t = QuerySplitter::tokenize(inQuery);
			auto qs = QuerySplitter(t);
			qs.parse();
			return qs;		
		}


	};
	testhelper QS = testhelper();

public:
	TEST_METHOD(Test_basicNoCondClausesRun) {
		auto inQuery = "assign a; Select a";
		auto qs = QS.makeQS(inQuery);

		std::vector<pair<std::string, std::string>> declarations_t = { make_pair("a", "assign") };
		std::vector<Token> select_t = QuerySplitter::tokenize("a");

		Assert::AreEqual(declarations_t, qs.getDeclarations());
		Assert::AreEqual(select_t, qs.getSelectArg());
	}
	
	TEST_METHOD(Test_basicSuchThatRun) {
		auto inQuery = "stmt a; Select a such that Next()";
		auto qs = QS.makeQS(inQuery);

		std::vector<pair<std::string, std::string>> declarations_t = { make_pair("a", "stmt") };
		std::vector<Token> select_t = QuerySplitter::tokenize("a");
		std::vector<std::vector<Token>> stClauses_t = {
			QuerySplitter::tokenize("Next()")
		};

		Assert::AreEqual(declarations_t, qs.getDeclarations());
		Assert::AreEqual(select_t, qs.getSelectArg());
		Assert::AreEqual(stClauses_t, qs.getSuchThatClauses());
	}

	TEST_METHOD(Test_basicPatternRun) {
		auto inQuery = "assign a; Select a pattern a(_,_\"hello\"_)";
		auto qs = QS.makeQS(inQuery);

		std::vector<pair<std::string, std::string>> declarations_t = {
			make_pair("a", "assign") 
		};
		std::vector<Token> select_t = QuerySplitter::tokenize("a");
		std::vector<std::vector<Token>> ptClauses_t = {
			QuerySplitter::tokenize("a(_,_\"hello\"_)")
		};

		Assert::AreEqual(declarations_t, qs.getDeclarations());
		Assert::AreEqual(select_t, qs.getSelectArg());
		Assert::AreEqual(ptClauses_t, qs.getPatternClauses());
	}

	TEST_METHOD(Test_basicWithRun) {
		auto inQuery = "constant c; Select c with c.value=3";
		auto qs = QS.makeQS(inQuery);
		std::vector<pair<std::string, std::string>> declarations_t = { make_pair("c", "constant") };

		Assert::IsTrue(qs.getDeclarations() == declarations_t);

		Assert::IsTrue(qs.getSelectArg() ==
			QuerySplitter::tokenize("c")
		);

		Assert::IsTrue(qs.getWithClauses() == clauses_t{
			QuerySplitter::tokenize("c.value=3")
			});
	}
	
	TEST_METHOD(Test_multiDeclare) {
		auto inQuery = "stmt a,b,c; stmt d; Select a such that Next()";
		auto qs = QS.makeQS(inQuery);
		std::vector<pair<std::string, std::string>> declarations_t = {
			make_pair("a", "stmt"),
			make_pair("b", "stmt"),
			make_pair("c", "stmt"),
			make_pair("d", "stmt")
		};

		Assert::IsTrue(qs.getDeclarations() == declarations_t);

	}

	TEST_METHOD(Test_declareSyntaxError) {
		Assert::ExpectException<QuerySyntaxException>([&]() {
			auto inQuery = "stmt a,; Select a such that Next()";
			auto qs = QS.makeQS(inQuery);
			});
		Assert::ExpectException<QuerySyntaxException>([&]() {
			auto inQuery = "stmta; Select a such that Next()";
			auto qs = QS.makeQS(inQuery);
			});
		Assert::ExpectException<QuerySyntaxException>([&]() {
			auto inQuery = "stmt a a; Select a such that Next();";
			auto qs = QS.makeQS(inQuery);
			});
	}

	TEST_METHOD(Test_SelectBOOLEAN_suchThat) {
		auto inQuery = "stmt a; Select BOOLEAN such that Next()";
		auto qs = QS.makeQS(inQuery);
		std::vector<pair<std::string, std::string>> declarations_t = { make_pair("a", "stmt") };
		Assert::IsTrue(qs.getDeclarations() == declarations_t);

		Assert::IsTrue(qs.getSelectArg() ==
			QuerySplitter::tokenize("BOOLEAN")
		);

		Assert::IsTrue(qs.getSuchThatClauses() == clauses_t {
			QuerySplitter::tokenize("Next()")
			});
	}

	TEST_METHOD(Test_SelectNameAttr_suchThat) {
		auto inQuery = "procedure p; Select p.procName such that Next()";
		auto qs = QS.makeQS(inQuery);

		std::vector<pair<std::string, std::string>> declarations_t = { make_pair("p", "procedure")};
		Assert::IsTrue(qs.getDeclarations() == declarations_t);

		Assert::IsTrue(qs.getSelectArg() ==
			QuerySplitter::tokenize("p.procName")
		);

		Assert::IsTrue(qs.getSuchThatClauses() == clauses_t{
			QuerySplitter::tokenize("Next()")
			});
	}

	  

	TEST_METHOD(Test_SelectNameAttrWHash_suchThat) {
		auto inQuery = "stmt s; Select s.stmt# such that Next()";
		auto qs = QS.makeQS(inQuery);

		std::vector<pair<std::string, std::string>> declarations_t = { make_pair("s", "stmt")};

		Assert::IsTrue(qs.getDeclarations() == declarations_t);

		Assert::IsTrue(qs.getSelectArg() ==
			QuerySplitter::tokenize("s.stmt#")
		);

		Assert::IsTrue(qs.getSuchThatClauses() == clauses_t{
			QuerySplitter::tokenize("Next()")
			});
	}

	TEST_METHOD(Test_SelectNameInvalidAttr_suchThat) {
		Assert::ExpectException<QuerySyntaxException>([&]() {
			auto inQuery = "stmt s; Select s.stmtNum such that Next()";
			auto qs = QS.makeQS(inQuery);
			});
	}

	TEST_METHOD(Test_SelectTuple_suchThat) {
		auto inQuery = "stmt a; Select <a,a,a> such that Next()";
		auto qs = QS.makeQS(inQuery);

		std::vector<pair<std::string, std::string>> declarations_t = { make_pair("a", "stmt")};

		Assert::IsTrue(qs.getDeclarations() == declarations_t);

		Assert::IsTrue(qs.getSelectArg() ==
			QuerySplitter::tokenize("<a,a,a>")
		);

		Assert::IsTrue(qs.getSuchThatClauses() == clauses_t {
			QuerySplitter::tokenize("Next()")
			});
	}

	TEST_METHOD(Test_SelectTupleDupl_suchThat) {
		auto inQuery = "stmt a; Select <a,a,a> such that Next()";
		auto qs = QS.makeQS(inQuery);

		std::vector<pair<std::string, std::string>> declarations_t = { make_pair("a", "stmt")};
		Assert::IsTrue(qs.getDeclarations() == declarations_t);

		Assert::IsTrue(qs.getSelectArg() ==
			QuerySplitter::tokenize("<a,a,a>")
		);

		Assert::IsTrue(qs.getSuchThatClauses() == clauses_t{
			QuerySplitter::tokenize("Next()")
			});
	}

	TEST_METHOD(Test_SelectDuplicateSuchSingleDecDupl_suchThat) {
		auto inQuery = "stmt such, such; Select such such that Next()";
		auto qs = QS.makeQS(inQuery);

		std::vector<pair<std::string, std::string>> declarations_t = { 
			make_pair("such", "stmt"),
			make_pair("such", "stmt")
		};
		Assert::IsTrue(qs.getDeclarations() == declarations_t);

		Assert::IsTrue(qs.getSelectArg() ==
			QuerySplitter::tokenize("such")
		);

		Assert::IsTrue(qs.getSuchThatClauses() == clauses_t{
			QuerySplitter::tokenize("Next()")
			});
	}

	TEST_METHOD(Test_SelectDuplicateSuchMultDecDupl_suchThat) {
		auto inQuery = "stmt such; variable such; Select such such that Next()";
		auto qs = QS.makeQS(inQuery);

		std::vector<pair<std::string, std::string>> declarations_t = { 
			make_pair("such", "stmt"),
			make_pair("such", "variable")
		};
		Assert::IsTrue(qs.getDeclarations() == declarations_t);

		Assert::IsTrue(qs.getSelectArg() ==
			QuerySplitter::tokenize("such")
		);

		Assert::IsTrue(qs.getSuchThatClauses() == clauses_t{
			QuerySplitter::tokenize("Next()")
			});
	}

	TEST_METHOD(Test_SelectTupleWAttr_suchThat) {
		auto inQuery = "stmt a; procedure p; Select <a,a.stmt#,p.procName> such that Next()";
		auto qs = QS.makeQS(inQuery);

		std::vector<pair<std::string, std::string>> declarations_t = { 
			make_pair("a", "stmt"),
			make_pair("p", "procedure"),
		};
		Assert::IsTrue(qs.getDeclarations() == declarations_t);

		Assert::IsTrue(qs.getSelectArg() ==
			QuerySplitter::tokenize("<a,a.stmt#,p.procName>")
		);

		Assert::IsTrue(qs.getSuchThatClauses() == clauses_t{
			QuerySplitter::tokenize("Next()")
			});
	}

	TEST_METHOD(Test_SelectSynNotInDeclarations_splitSucceed) {
		auto inQuery = "stmt a; Select b such that Next()";
		auto qs = QS.makeQS(inQuery);

		std::vector<pair<std::string, std::string>> declarations_t = { make_pair("a", "stmt")};
		Assert::IsTrue(qs.getDeclarations() == declarations_t);

		Assert::IsTrue(qs.getSelectArg() ==
			QuerySplitter::tokenize("b")
		);

		Assert::IsTrue(qs.getSuchThatClauses() == clauses_t{
			QuerySplitter::tokenize("Next()")
			});
	}

	TEST_METHOD(Test_SelectTupleWithSynNotInDeclarations_splitSucceed) {
		auto inQuery = "stmt a; Select <b,a> such that Next()";
		auto qs = QS.makeQS(inQuery);

		std::vector<pair<std::string, std::string>> declarations_t = { make_pair("a", "stmt")};
		Assert::IsTrue(qs.getDeclarations() == declarations_t);

		Assert::IsTrue(qs.getSelectArg() ==
			QuerySplitter::tokenize("<b,a>")
		);

		Assert::IsTrue(qs.getSuchThatClauses() == clauses_t{
			QuerySplitter::tokenize("Next()")
			});
	}

	TEST_METHOD(Test_SelectErrors) {
		Assert::ExpectException<QuerySyntaxException>([&]() {
			auto inQuery = "stmt a; Select a such that Next();";
			auto qs = QS.makeQS(inQuery);
			});
		Assert::ExpectException<QuerySyntaxException>([&]() {
			auto inQuery = "stmt a; Select such that Next()";
			auto qs = QS.makeQS(inQuery);
		});
		Assert::ExpectException<QuerySyntaxException>([&]() {
			auto inQuery = "stmt such; Select such that Next()";
			auto qs = QS.makeQS(inQuery);
		});
		Assert::ExpectException<QuerySyntaxException>([&]() {
			auto inQuery = "stmt a; Select <a,a,a such that Next()";
			auto qs = QS.makeQS(inQuery);
		});
		Assert::ExpectException<QuerySyntaxException>([&]() {
			auto inQuery = "stmt a; Select a,a,a> such that Next()";
			auto qs = QS.makeQS(inQuery);
			});
		Assert::ExpectException<QuerySyntaxException>([&]() {
			auto inQuery = "stmt a; Select a,a such that Next()";
			auto qs = QS.makeQS(inQuery);
			});
		Assert::ExpectException<QuerySyntaxException>([&]() {
			auto inQuery = "stmt a; Select ,a such that Next()";
			auto qs = QS.makeQS(inQuery);
			});
		
		Assert::ExpectException<QuerySyntaxException>([&]() {
			auto inQuery = "stmt such; Select such that Next()";
			auto qs = QS.makeQS(inQuery);
			});

		/*Assert::ExpectException<QuerySemanticException>([&]() {
			auto inQuery = "stmt such; variable such; Select such such that Next()";
			auto qs = QS.makeQS(inQuery);
			});

		Assert::ExpectException<QuerySemanticException>([&]() {
			auto inQuery = "stmt such, such; Select such such that Next()";
			auto qs = QS.makeQS(inQuery);
			});*/

	}
	TEST_METHOD(Test_multiClauseAnd_suchThat) {
		auto inQuery = "stmt a; stmt d; Select a "
			"such that Next() and Next()";
		auto qs = QS.makeQS(inQuery);


		Assert::IsTrue(qs.getSuchThatClauses() == clauses_t {
			QuerySplitter::tokenize("Next()"),
			QuerySplitter::tokenize("Next()")
			});
	}

	TEST_METHOD(Test_multiClauseAnd_Pattern) {
		auto inQuery = "stmt a; stmt d; Select a "
			"pattern a() and b()";
		auto qs = QS.makeQS(inQuery);

		Assert::IsTrue(qs.getPatternClauses() == clauses_t {
			QuerySplitter::tokenize("a()"),
			QuerySplitter::tokenize("b()")
			});
	}

	TEST_METHOD(Test_multiClauseAnd_With) {
		auto inQuery = "stmt a; Select a "
			"with a.procName=a.procName and 1=1";
		auto qs = QS.makeQS(inQuery);

		std::vector<pair<std::string, std::string>> declarations_t = { make_pair("a", "stmt")};
		Assert::IsTrue(qs.getDeclarations() == declarations_t);

		Assert::IsTrue(qs.getSelectArg() == QuerySplitter::tokenize("a"));

		vector<vector<Token>> clauses_t = {
			QuerySplitter::tokenize("a.procName=a.procName"),
			QuerySplitter::tokenize("1=1")
		};

		Assert::AreEqual(clauses_t, qs.getWithClauses());
	}

	TEST_METHOD(Test_multiClauseMix) {
		auto inQuery = "stmt a; Select a "
			"with a.procName=a.procName and 1=1 "
			" such that Next() pattern a(ab) and b(cc)";
		auto qs = QS.makeQS(inQuery);

		Assert::IsTrue(qs.getSuchThatClauses() == clauses_t {
			QuerySplitter::tokenize("Next()")
			});

		Assert::IsTrue(qs.getPatternClauses() == clauses_t{
			QuerySplitter::tokenize("a(ab)"),
			QuerySplitter::tokenize("b(cc)"),
			});

		Assert::IsTrue(qs.getWithClauses() == clauses_t{
			QuerySplitter::tokenize("a.procName=a.procName"),
			QuerySplitter::tokenize("1=1"),
			});
	}
	TEST_METHOD(Test_whileSuchThatParent) {
		auto inQuery = "while w; Select w such that Parent(w,10)";
		auto qs = QS.makeQS(inQuery);

		std::vector<pair<std::string, std::string>> declarations_t = { make_pair("w", "while")};
		Assert::IsTrue(qs.getDeclarations() == declarations_t);

		Assert::IsTrue(qs.getSelectArg() ==
			QuerySplitter::tokenize("w")
		);

		Assert::IsTrue(qs.getSuchThatClauses() == clauses_t{
			QuerySplitter::tokenize("Parent(w,10)")
			});
	}

	TEST_METHOD(Test_specialCaseBooleanSynOnly) {
		auto inQuery = "stmt BOOLEAN; Select BOOLEAN";
		auto qs = QS.makeQS(inQuery);

		std::vector<pair<std::string, std::string>> declarations_t = {
			make_pair("BOOLEAN", "stmt")
		};
		std::vector<Token> select_t = QuerySplitter::tokenize("BOOLEAN");

		Assert::AreEqual(declarations_t, qs.getDeclarations());
		Assert::AreEqual(select_t, qs.getSelectArg());
	}

	TEST_METHOD(Test_specialCaseBooleanSynMultiMix) {
		auto inQuery = "stmt BOOLEAN; assign a; Select BOOLEAN such that Modifies(BOOLEAN, _) pattern a(_, BOOLEAN) with BOOLEAN.stmt#=3";
		auto qs = QS.makeQS(inQuery);

		std::vector<pair<std::string, std::string>> declarations_t = { 
			make_pair("BOOLEAN","stmt"),
			make_pair("a","assign")
		};
		std::vector<Token> select_t = QuerySplitter::tokenize("BOOLEAN");
		std::vector<std::vector<Token>> stClause_t = { QuerySplitter::tokenize("Modifies(BOOLEAN, _)") };
		std::vector<std::vector<Token>> ptClause_t = { QuerySplitter::tokenize("a(_, BOOLEAN)") };
		std::vector<std::vector<Token>> withClause_t = { QuerySplitter::tokenize("BOOLEAN.stmt#=3") };

		Assert::AreEqual(declarations_t, qs.getDeclarations());
		Assert::AreEqual(select_t, qs.getSelectArg());
		Assert::AreEqual(stClause_t, qs.getSuchThatClauses());
		Assert::AreEqual(ptClause_t, qs.getPatternClauses());
		Assert::AreEqual(withClause_t, qs.getWithClauses());
	}
};
}