#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/QP/QuerySplitter/QuerySplitter.h"
#include "../source/QP/QueryParser/QueryParser.h"
#include "../source/QP/QueryObject/Declaration.h"
#include "../source/QP/QueryObject/QueryObject.h"
#include "../source/QP/QueryObject/SelectObject.h"
#include "../source/QP/QueryObject/PatternClause.h"
#include "../source/QP/QueryObject/DesignAbstractionFactory.h"
#include "../source/QP/QueryExceptions/QuerySyntaxException.h"
#include "../source/QP/QueryExceptions/QuerySemanticException.h"
#include "../source/SourceProcessor/Tokenizer/SPTokenizer.h"
#include <codecvt>
#include <locale>
#include <vector>
#include <unordered_map>

namespace Microsoft {
	namespace VisualStudio {
		namespace CppUnitTestFramework {
			auto pattern_converter = std::wstring_convert<std::codecvt_utf8<wchar_t>>();

			static std::wstring comparePatternClauseStr(const PatternClause& patternCl) {
				//ArgsType argsType = patternCl.arg1;
				std::string result = "Pattern Clause";
				return pattern_converter.from_bytes(result);
			}

			static std::wstring comparePatternClauses(const vector<PatternClause>& patternClauses) {
				std::string result = "PatternClauses list containing pattern-clauses: ";
				for (PatternClause patternCl : patternClauses) {
					std::wstring patternClWStr = comparePatternClauseStr(patternCl);
					result += pattern_converter.to_bytes(patternClWStr);
				}
				return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(result);
			}

			template<>
			static std::wstring ToString<PatternClause>(const PatternClause& patternCl) {
				return comparePatternClauseStr(patternCl);
			}

			template<>
			static std::wstring ToString<vector<PatternClause>>(const vector<PatternClause>& patternClauses) {
				return comparePatternClauses(patternClauses);
			}

			template<>
			static std::wstring ToString<PatternClauses>(const PatternClauses& patternClauses) {
				auto wStr1 = comparePatternClauses(patternClauses.noSynClauses);
				auto wStr2 = comparePatternClauses(patternClauses.commonSynClauses);
				return wStr1.append(wStr2);
			}
		}
	}
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestQPPatternParser) {
public:
	/* PATTERN (assign) - valid */
	TEST_METHOD(Test_validAssignPatternWildcardWildcardParse) {
		string query = "a(_, _)";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration keyDec = Declaration(make_shared<DE::Assign>(), "a");
		std::vector<Declaration> inputDecs = { keyDec };
		struct Argument arg1 = { WildCard() };
		struct Argument arg2 = { WildCard() };
		std::vector<PatternClause> expectedPtCl = { PatternClause(keyDec, arg1, arg2) };
		struct PatternClauses expectedPatternClauses = { {}, expectedPtCl };

		PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
		Assert::AreEqual(expectedPatternClauses, patternResult);
	}

	TEST_METHOD(Test_validAssignPatternSynWildcardParse) {
		string query = "a(v, _)";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration assignDec = Declaration(make_shared<DE::Assign>(), "a");
		Declaration varDec = Declaration(make_shared<DE::Variable>(), "v");
		std::vector<Declaration> inputDecs = { assignDec, varDec };
		struct Argument arg1 = { varDec };
		struct Argument arg2 = { WildCard() };
		std::vector<PatternClause> expectedPtCl = { PatternClause(assignDec, arg1, arg2) };
		struct PatternClauses expectedPatternClauses = { {}, expectedPtCl };

		PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
		Assert::AreEqual(expectedPatternClauses, patternResult);
	}

	TEST_METHOD(Test_validAssignPatternStringWildcardParse) {
		string query = "a(\"x\", _)";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration assignDec = Declaration(make_shared<DE::Assign>(), "a");
		std::vector<Declaration> inputDecs = { assignDec };
		struct Argument arg1 = { StringExp("x") };
		struct Argument arg2 = { WildCard() };
		std::vector<PatternClause> expectedPtCl = { PatternClause(assignDec, arg1, arg2) };
		struct PatternClauses expectedPatternClauses = { {}, expectedPtCl };

		PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
		Assert::AreEqual(expectedPatternClauses, patternResult);
	}

	TEST_METHOD(Test_validAssignPatternWildcardStrEqParse) {
		string query = "a(_, \"(x + y) * z / 1 - x % y\")";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration assignDec = Declaration(make_shared<DE::Assign>(), "a");
		std::vector<Declaration> inputDecs = { assignDec };
		struct Argument arg1 = { WildCard() };
		struct Argument arg2 = { StringExp("(x+y)*z/1-x%y") };
		std::vector<PatternClause> expectedPtCl = { PatternClause(assignDec, arg1, arg2) };
		struct PatternClauses expectedPatternClauses = { {}, expectedPtCl };

		PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
		Assert::AreEqual(expectedPatternClauses, patternResult);
	}

	TEST_METHOD(Test_validAssignPatternVarStrEqParse) {
		string query = "a(v, \"x + y * z / 1 - x % y\")";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration assignDec = Declaration(make_shared<DE::Assign>(), "a");
		Declaration varDec = Declaration(make_shared<DE::Variable>(), "v");
		std::vector<Declaration> inputDecs = { assignDec, varDec };
		struct Argument arg1 = { varDec };
		struct Argument arg2 = { StringExp("x+y*z/1-x%y") };
		std::vector<PatternClause> expectedPtCl = { PatternClause(assignDec, arg1, arg2) };
		struct PatternClauses expectedPatternClauses = { {}, expectedPtCl };

		PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
		Assert::AreEqual(expectedPatternClauses, patternResult);
	}

	TEST_METHOD(Test_validAssignPatternStringStrEqParse) {
		string query = "a(\"z\", \"1 + 1 - x % y % (x + y) * z\")";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration assignDec = Declaration(make_shared<DE::Assign>(), "a");
		std::vector<Declaration> inputDecs = { assignDec };
		struct Argument arg1 = { StringExp("z") };
		struct Argument arg2 = { StringExp("1+1-x%y%(x+y)*z") };
		std::vector<PatternClause> expectedPtCl = { PatternClause(assignDec, arg1, arg2) };
		struct PatternClauses expectedPatternClauses = { {}, expectedPtCl };

		PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
		Assert::AreEqual(expectedPatternClauses, patternResult);
	}

	TEST_METHOD(Test_validAssignPatternStringStrEqVarParse) {
		string query = "a(\"z\", \"a\")";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration assignDec = Declaration(make_shared<DE::Assign>(), "a");
		Declaration constDec = Declaration(make_shared<DE::Constant>(), "z");
		std::vector<Declaration> inputDecs = { assignDec, constDec };
		struct Argument arg1 = { StringExp("z") };
		struct Argument arg2 = { StringExp("a") };
		std::vector<PatternClause> expectedPtCl = { PatternClause(assignDec, arg1, arg2) };
		struct PatternClauses expectedPatternClauses = { {}, expectedPtCl };

		PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
		Assert::AreEqual(expectedPatternClauses, patternResult);
	}

	TEST_METHOD(Test_validAssignPatternWildcardSubEqParse) {
		string query = "a(_, _\"z / 1 - x % y\"_)";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration assignDec = Declaration(make_shared<DE::Assign>(), "a");
		std::vector<Declaration> inputDecs = { assignDec };
		struct Argument arg1 = { WildCard() };
		struct Argument arg2 = { SubExp("z/1-x%y") };
		std::vector<PatternClause> expectedPtCl = { PatternClause(assignDec, arg1, arg2) };
		struct PatternClauses expectedPatternClauses = { {}, expectedPtCl };

		PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
		Assert::AreEqual(expectedPatternClauses, patternResult);
	}

	TEST_METHOD(Test_validAssignPatternVarSubEqParse) {
		string query = "a(v, _\"(x + y)\"_)";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration assignDec = Declaration(make_shared<DE::Assign>(), "a");
		Declaration varDec = Declaration(make_shared<DE::Variable>(), "v");
		std::vector<Declaration> inputDecs = { assignDec, varDec };
		struct Argument arg1 = { varDec };
		struct Argument arg2 = { SubExp("(x+y)") };
		std::vector<PatternClause> expectedPtCl = { PatternClause(assignDec, arg1, arg2) };
		struct PatternClauses expectedPatternClauses = { {}, expectedPtCl };

		PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
		Assert::AreEqual(expectedPatternClauses, patternResult);
	}

	TEST_METHOD(Test_validAssignPatternVarSubEqConstParse) {
		string query = "a(v, _\"1\"_)";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration assignDec = Declaration(make_shared<DE::Assign>(), "a");
		Declaration varDec = Declaration(make_shared<DE::Variable>(), "v");
		std::vector<Declaration> inputDecs = { assignDec, varDec };
		struct Argument arg1 = { varDec };
		struct Argument arg2 = { SubExp("1") };
		std::vector<PatternClause> expectedPtCl = { PatternClause(assignDec, arg1, arg2) };
		struct PatternClauses expectedPatternClauses = { {}, expectedPtCl };

		PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
		Assert::AreEqual(expectedPatternClauses, patternResult);
	}

	TEST_METHOD(Test_validAssignPatternVarSubEqVarParse) {
		string query = "a(v, _\"a\"_)";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration assignDec = Declaration(make_shared<DE::Assign>(), "a");
		Declaration varDec = Declaration(make_shared<DE::Variable>(), "v");
		std::vector<Declaration> inputDecs = { assignDec, varDec };
		struct Argument arg1 = { varDec };
		struct Argument arg2 = { SubExp("a") };
		std::vector<PatternClause> expectedPtCl = { PatternClause(assignDec, arg1, arg2) };
		struct PatternClauses expectedPatternClauses = { {}, expectedPtCl };

		PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
		Assert::AreEqual(expectedPatternClauses, patternResult);
	}

	/* PATTERN (assign) - invalid */
	TEST_METHOD(Test_invalidAssignPatternExprParse) {
		string query = "a(_, \"(x ++ y)\")";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		std::vector<Declaration> inputDecs = {
			Declaration(make_shared<DE::Assign>(), "a")
		};

		Assert::ExpectException<QuerySyntaxException>([&]() {
			PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidAssignPatternSynTypeAndExprParse) {
		string query = "s(_, \"(x ++ y)\")";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		std::vector<Declaration> inputDecs = {
			Declaration(make_shared<DE::Statement>(), "s")
		};

		Assert::ExpectException<QuerySyntaxException>([&]() {
			PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidAssignPatternExprBracketsParse) {
		Assert::ExpectException<QuerySyntaxException>([&]() {
			string query = "a(_, \"(x + y)))\")";
			std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
			std::vector<Declaration> inputDecs = {
				Declaration(make_shared<DE::Assign>(), "a")
			};
			PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidAssignPatternExprTermSymbolParse) {
		Assert::ExpectException<QuerySyntaxException>([&]() {
			string query = "a(_, \"(#x + y)))\")";
			std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
			std::vector<Declaration> inputDecs = {
				Declaration(make_shared<DE::Assign>(), "a")
			};
			PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidAssignPatternInvalidSynRefPatternParse) {
		Assert::ExpectException<QuerySemanticException>([&]() {
			string query = "b(\"x\", \"(x + y)\")";
			std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
			std::vector<Declaration> inputDecs = {
				Declaration(make_shared<DE::Assign>(), "a")
			};
			PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidAssignPatternNameParse) {
		Assert::ExpectException<QuerySyntaxException>([&]() {
			string query = "a(1bas, \"(x + y)))\")";
			std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
			std::vector<Declaration> inputDecs = {
				Declaration(make_shared<DE::Assign>(), "a")
			};
			PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidAssignPatternSubExpParse) {
		Assert::ExpectException<QuerySyntaxException>([&]() {
			string query = "a(_\"subExp\"_, \"(x + y)))\")";
			std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
			std::vector<Declaration> inputDecs = {
				Declaration(make_shared<DE::Assign>(), "a")
			};
			PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidAssignPatternStrExpParse) {
		Assert::ExpectException<QuerySyntaxException>([&]() {
			string query = "a(\"x + y\", \"(x + y)))\")";
			std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
			std::vector<Declaration> inputDecs = {
				Declaration(make_shared<DE::Assign>(), "a")
			};
			PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidAssignPatternMissingStrDelimParse) {
		Assert::ExpectException<QuerySyntaxException>([&]() {
			string query = "a(v, (x + y))";
			std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
			std::vector<Declaration> inputDecs = {
				Declaration(make_shared<DE::Assign>(), "a"),
				Declaration(make_shared<DE::Variable>(), "v")
			};
			PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
			});
	}

	/* PATTERN (while) - valid */
	TEST_METHOD(Test_validWhilePatternWildcardWildcardParse) {
		string query = "w(_, _)";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration keyDec = Declaration(make_shared<DE::While>(), "w");
		std::vector<Declaration> inputDecs = { keyDec };
		struct Argument arg1 = { WildCard() };
		struct Argument arg2 = { WildCard() };
		std::vector<PatternClause> expectedPtCl = { PatternClause(keyDec, arg1, arg2) };
		struct PatternClauses expectedPatternClauses = { {}, expectedPtCl };

		PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
		Assert::AreEqual(expectedPatternClauses, patternResult);
	}

	TEST_METHOD(Test_validWhilePatternStrWildcardParse) {
		string query = "w(\"w\", _)";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration keyDec = Declaration(make_shared<DE::While>(), "w");
		std::vector<Declaration> inputDecs = { keyDec };
		struct Argument arg1 = { StringExp("w") };
		struct Argument arg2 = { WildCard() };
		std::vector<PatternClause> expectedPtCl = { PatternClause(keyDec, arg1, arg2) };
		struct PatternClauses expectedPatternClauses = { {}, expectedPtCl };

		PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
		Assert::AreEqual(expectedPatternClauses, patternResult);
	}

	TEST_METHOD(Test_validWhilePatternVarWildcardParse) {
		string query = "w(v, _)";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration whileDec = Declaration(make_shared<DE::While>(), "w");
		Declaration varDec = Declaration(make_shared<DE::Variable>(), "v");
		std::vector<Declaration> inputDecs = { whileDec, varDec };
		struct Argument arg1 = { varDec };
		struct Argument arg2 = { WildCard() };
		std::vector<PatternClause> expectedPtCl = { PatternClause(whileDec, arg1, arg2) };
		struct PatternClauses expectedPatternClauses = { {}, expectedPtCl };

		PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
		Assert::AreEqual(expectedPatternClauses, patternResult);
	}

	/* PATTERN (while) - invalid */

	TEST_METHOD(Test_invalidWhilePatternNonVarSynParse) {
		string query = "w(w, _)";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		std::vector<Declaration> inputDecs = {
			Declaration(make_shared<DE::While>(), "w")
		};

		Assert::ExpectException<QuerySemanticException>([&]() {
			PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidWhilePatternThreeArgCountParse) {
		string query = "w(_, _, _)";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		std::vector<Declaration> inputDecs = {
			Declaration(make_shared<DE::While>(), "w")
		};

		Assert::ExpectException<QuerySemanticException>([&]() {
			PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidWhilePatternArgCountExcParse) {
		string query = "w(_, _, _, _)";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		std::vector<Declaration> inputDecs = {
			Declaration(make_shared<DE::While>(), "w")
		};

		Assert::ExpectException<QuerySyntaxException>([&]() {
			PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidWhilePatternSubEqParse) {
		string query = "w(v, _\"x + y\"_)";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		std::vector<Declaration> inputDecs = {
			Declaration(make_shared<DE::While>(), "w"),
			Declaration(make_shared<DE::Variable>(), "v"),
		};

		Assert::ExpectException<QuerySemanticException>([&]() {
			PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidWhilePatternInvalidStrParse) {
		string query = "w(\"123\", _)";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		std::vector<Declaration> inputDecs = {
			Declaration(make_shared<DE::While>(), "w")
		};

		Assert::ExpectException<QuerySyntaxException>([&]() {
			PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidWhilePatternStrConstParse) {
		string query = "w(\"w\", 123)";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		std::vector<Declaration> inputDecs = {
			Declaration(make_shared<DE::While>(), "w")
		};

		Assert::ExpectException<QuerySyntaxException>([&]() {
			PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidWhilePatternStrVarParse) {
		string query = "w(\"w\", w)";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		std::vector<Declaration> inputDecs = {
			Declaration(make_shared<DE::While>(), "w")
		};

		Assert::ExpectException<QuerySyntaxException>([&]() {
			PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidWhilePatternStrEqParse) {
		string query = "w(\"w\", \"x + y * z / 1 - x % y\")";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		std::vector<Declaration> inputDecs = {
			Declaration(make_shared<DE::While>(), "w")
		};

		Assert::ExpectException<QuerySemanticException>([&]() {
			PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidWhilePatternNotVarSynAndStrEqParse) {
		string query = "w(ifs, \"x + y * z / 1 - x % y\")";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		std::vector<Declaration> inputDecs = {
			Declaration(make_shared<DE::While>(), "w"),
			Declaration(make_shared<DE::If>(), "ifs"),
		};

		Assert::ExpectException<QuerySemanticException>([&]() {
			PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
			});
	}

	/* PATTERN (if) - valid */
	TEST_METHOD(Test_validIfPatternWildcardWildcardParse) {
		string query = "if1(_, _, _)";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration keyDec = Declaration(make_shared<DE::If>(), "if1");
		std::vector<Declaration> inputDecs = { keyDec };
		struct Argument arg1 = { WildCard() };
		struct Argument arg2 = { WildCard() };
		std::vector<PatternClause> expectedPtCl = { PatternClause(keyDec, arg1, arg2) };
		struct PatternClauses expectedPatternClauses = { {}, expectedPtCl };

		PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
		Assert::AreEqual(expectedPatternClauses, patternResult);
	}

	TEST_METHOD(Test_validIfPatternStrWildcardParse) {
		string query = "ifs(\"ifs\", _, _)";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration keyDec = Declaration(make_shared<DE::If>(), "ifs");
		std::vector<Declaration> inputDecs = { keyDec };
		struct Argument arg1 = { StringExp("ifs") };
		struct Argument arg2 = { WildCard() };
		std::vector<PatternClause> expectedPtCl = { PatternClause(keyDec, arg1, arg2) };
		struct PatternClauses expectedPatternClauses = { {}, expectedPtCl };

		PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
		Assert::AreEqual(expectedPatternClauses, patternResult);
	}

	TEST_METHOD(Test_validIfPatternVarWildcardParse) {
		string query = "if(v, _, _)";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration whileDec = Declaration(make_shared<DE::If>(), "if");
		Declaration varDec = Declaration(make_shared<DE::Variable>(), "v");
		std::vector<Declaration> inputDecs = { whileDec, varDec };
		struct Argument arg1 = { varDec };
		struct Argument arg2 = { WildCard() };
		std::vector<PatternClause> expectedPtCl = { PatternClause(whileDec, arg1, arg2) };
		struct PatternClauses expectedPatternClauses = { {}, expectedPtCl };

		PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
		Assert::AreEqual(expectedPatternClauses, patternResult);
	}

	/* PATTERN (if) - invalid */
	TEST_METHOD(Test_invalidIfPatternNonVarSynParse) {
		string query = "if(c, _, _)";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		std::vector<Declaration> inputDecs = {
			Declaration(make_shared<DE::If>(), "if"),
			Declaration(make_shared<DE::Constant>(), "c")
		};

		Assert::ExpectException<QuerySemanticException>([&]() {
			PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidIfPatternTwoArgCountParse) {
		string query = "i(_, _)";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		std::vector<Declaration> inputDecs = {
			Declaration(make_shared<DE::If>(), "i")
		};

		Assert::ExpectException<QuerySemanticException>([&]() {
			PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidIfPatternArgCountExcParse) {
		string query = "ifs(_, _, _, _)";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		std::vector<Declaration> inputDecs = {
			Declaration(make_shared<DE::If>(), "ifs")
		};

		Assert::ExpectException<QuerySyntaxException>([&]() {
			PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidIfPatternMissingDelimParse) {
		string query = "ifs(_, _ _)";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		std::vector<Declaration> inputDecs = {
			Declaration(make_shared<DE::If>(), "ifs")
		};

		Assert::ExpectException<QuerySyntaxException>([&]() {
			PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidIfPatternSubEqParse) {
		string query = "ifs(v, _, _\"x + y\"_)";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		std::vector<Declaration> inputDecs = {
			Declaration(make_shared<DE::If>(), "ifs"),
			Declaration(make_shared<DE::Variable>(), "v"),
		};

		Assert::ExpectException<QuerySyntaxException>([&]() {
			PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidIfPatternInvalidStrParse) {
		string query = "if(\"123\", _, _)";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		std::vector<Declaration> inputDecs = {
			Declaration(make_shared<DE::If>(), "if")
		};

		Assert::ExpectException<QuerySyntaxException>([&]() {
			PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidIfPatternStrConstParse) {
		string query = "ifs(\"ifs\", _,123)";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		std::vector<Declaration> inputDecs = {
			Declaration(make_shared<DE::If>(), "ifs")
		};

		Assert::ExpectException<QuerySyntaxException>([&]() {
			PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidIfPatternStrVarParse) {
		string query = "ifs(\"ifs\", ifs, _)";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		std::vector<Declaration> inputDecs = {
			Declaration(make_shared<DE::If>(), "ifs")
		};

		Assert::ExpectException<QuerySyntaxException>([&]() {
			PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidIfPatternStrEqParse) {
		string query = "ifs(\"ifs\", \"x + y * z / 1 - x % y\", _)";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		std::vector<Declaration> inputDecs = {
			Declaration(make_shared<DE::If>(), "ifs")
		};

		Assert::ExpectException<QuerySyntaxException>([&]() {
			PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidIfPatternNotVarSynAndStrEqParse) {
		string query = "ifs(w, \"x + y * z / 1 - x % y\")";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		std::vector<Declaration> inputDecs = {
			Declaration(make_shared<DE::While>(), "w"),
			Declaration(make_shared<DE::If>(), "ifs"),
		};

		Assert::ExpectException<QuerySemanticException>([&]() {
			PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
			});
	}

	/* MISC - invalid */
	TEST_METHOD(Test_invalidPatternSynTypeInvalidEqParse) {
		string query = "s(_, \"(x + y)\")";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		std::vector<Declaration> inputDecs = {
			Declaration(make_shared<DE::Statement>(), "s")
		};

		Assert::ExpectException<QuerySemanticException>([&]() {
			PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidPatternNoSuchSynParse) {
		string query = "a(b, \"(x + y)\")";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		std::vector<Declaration> inputDecs = {
			Declaration(make_shared<DE::Assign>(), "a")
		};

		Assert::ExpectException<QuerySyntaxException>([&]() {
			PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidPatternFirstArgNotVarParse) {
		string query = "w(b, \"(x + y)\")";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		std::vector<Declaration> inputDecs = {
			Declaration(make_shared<DE::While>(), "w"),
			Declaration(make_shared<DE::Constant>(), "b")
		};

		Assert::ExpectException<QuerySemanticException>([&]() {
			PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidPatternSynTypeValidWildcardParse) {
		string query = "s(_, _)";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		std::vector<Declaration> inputDecs = {
			Declaration(make_shared<DE::Statement>(), "s")
		};

		Assert::ExpectException<QuerySemanticException>([&]() {
			PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidPatternArgsParse) {
		string query = "a(_, _, _, _)";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		std::vector<Declaration> inputDecs = {
			Declaration(make_shared<DE::Assign>(), "a")
		};

		Assert::ExpectException<QuerySyntaxException>([&]() {
			PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidPatternMissingStrDelimParse) {
		Assert::ExpectException<QuerySyntaxException>([&]() {
			string query = "ifs(v, (x + y))";
			std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
			std::vector<Declaration> inputDecs = {
				Declaration(make_shared<DE::If>(), "ifs"),
				Declaration(make_shared<DE::Variable>(), "v")
			};
			PatternClauses patternResult = PatternParser::parse({ inputTokens }, inputDecs);
			});
	}
	};
}