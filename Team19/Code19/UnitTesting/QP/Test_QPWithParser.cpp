#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/QP/QuerySplitter/QuerySplitter.h"
#include "../source/QP/QueryParser/QueryParser.h"
#include "../source/QP/QueryObject/Declaration.h"
#include "../source/QP/QueryObject/QueryObject.h"
#include "../source/QP/QueryObject/SelectObject.h"
#include "../source/QP/QueryObject/WithClause.h"
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
			auto with_converter = std::wstring_convert<std::codecvt_utf8<wchar_t>>();

			static std::wstring compareWithClauseStr(const WithClause& withCl) {
				//ArgsType argsType = withCl.arg1;
				std::string result = "With Clause";
				return with_converter.from_bytes(result);
			}

			static std::wstring compareWithClauses(const vector<WithClause>& withClauses) {
				std::string result = "WithClauses list containing with-clauses: ";
				for (WithClause withCl : withClauses) {
					std::wstring withClWStr = compareWithClauseStr(withCl);
					result += with_converter.to_bytes(withClWStr);
				}
				return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(result);
			}

			template<>
			static std::wstring ToString<WithClause>(const WithClause& withCl) {
				return compareWithClauseStr(withCl);
			}

			template<>
			static std::wstring ToString<vector<WithClause>>(const vector<WithClause>& withClauses) {
				return compareWithClauses(withClauses);
			}

			template<>
			static std::wstring ToString<WithClauses>(const WithClauses& withClauses) {
				auto wStr1 = compareWithClauses(withClauses.noSynClauses);
				auto wStr2 = compareWithClauses(withClauses.commonSynClauses);
				return wStr1.append(wStr2);
			}
		}
	}
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestQPWithParser) {
public:

	/* No AttrRef - errors only */
	TEST_METHOD(Test_invalidParamCompareThrowException) {
		string query = "\"string1\"=string2";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		std::vector<Declaration> inputDecs = {};

		Assert::ExpectException<QuerySyntaxException>([&]() {
			WithClauses withResult = WithParser::parse({ inputTokens }, inputDecs);
			});
	}

	TEST_METHOD(Test_noSynStrIntCompareThrowException) {
		string query = "\"string1\"=3";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		std::vector<Declaration> inputDecs = {};

		Assert::ExpectException<QuerySemanticException>([&]() {
			WithClauses withResult = WithParser::parse({ inputTokens }, inputDecs);
			});
	}

	TEST_METHOD(Test_wildcardCompareThrowException) {
		string query = "\"string1\"=_";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		std::vector<Declaration> inputDecs = {};

		Assert::ExpectException<QuerySyntaxException>([&]() {
			WithClauses withResult = WithParser::parse({ inputTokens }, inputDecs);
			});
	}



	/* No AttrRef - successful parsing */
	TEST_METHOD(Test_noSynStrCompareParseWithClauses) {
		string query = "\"string1\"=\"string2\"";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		struct Argument arg1 = { StringExp("string1")};
		struct Argument arg2 = { StringExp("string2")};
		WithClause withCl = WithClause(arg1, arg2);
		struct WithClauses expectedWithCl = { { withCl }, {} };

		std::vector<Declaration> inputDecs = {};
		WithClauses withResult = WithParser::parse({ inputTokens }, inputDecs);
		Assert::IsTrue(expectedWithCl == withResult);
	}

	TEST_METHOD(Test_noSynIntCompareParseWithClauses) {
		string query = "3=4";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		struct Argument arg1 = { Integer("3") };
		struct Argument arg2 = { Integer("4") };
		WithClause withCl = WithClause(arg1, arg2);
		struct WithClauses expectedWithCl = { { withCl }, {} };

		std::vector<Declaration> inputDecs = {};
		WithClauses withResult = WithParser::parse({ inputTokens }, inputDecs);
		Assert::IsTrue(expectedWithCl == withResult);
	}



	/* attrRef parsing - errors only */
	TEST_METHOD(Test_synNoAttrRefThrowException) {
		string query = "\"string1\"=r";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		std::vector<Declaration> inputDecs = { Declaration(make_shared<DE::Read>(), "r") };

		Assert::ExpectException<QuerySyntaxException>([&]() {
			WithClauses withResult = WithParser::parse({ inputTokens }, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidAttrRefThrowException) {
		string query = "\"string1\"=r.procName";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		std::vector<Declaration> inputDecs = { Declaration(make_shared<DE::Read>(), "r") };

		Assert::ExpectException<QuerySemanticException>([&]() {
			WithClauses withResult = WithParser::parse({ inputTokens }, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidStrAsParamCompareThrowException) {
		string query = "s.stmt#=\"3\"";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		std::vector<Declaration> inputDecs = { Declaration(make_shared<DE::Statement>(), "s") };

		Assert::ExpectException<QuerySyntaxException>([&]() {
			WithClauses withResult = WithParser::parse({ inputTokens }, inputDecs);
			});
	}

	TEST_METHOD(Test_validStrAsParamCompareThrowException) {
		string query = "s.stmt#  = \"var\"";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		std::vector<Declaration> inputDecs = { Declaration(make_shared<DE::Statement>(), "s") };

		Assert::ExpectException<QuerySemanticException>([&]() {
			WithClauses withResult = WithParser::parse({ inputTokens }, inputDecs);
			});
	}

	/* attrRef used - successful parsing*/
	TEST_METHOD(Test_sameTypeVarNameParseWithClauses) {
		string query = "\"string1\"=r.varName";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration readDec = Declaration(make_shared<DE::Read>(), "r");
		std::vector<Declaration> inputDecs = { readDec };
		struct SynAttr rhsSynAttr = { readDec, ArgType::ATTR_REF_NAME };
		struct Argument rhsArg = { rhsSynAttr };
		struct Argument lhsArg = { StringExp("string1") };

		WithClause withCl = WithClause(lhsArg, rhsArg);
		struct WithClauses expectedWithCl = { {}, { withCl } };
		WithClauses withResult = WithParser::parse({ inputTokens }, inputDecs);
		Assert::AreEqual(expectedWithCl, withResult);
	}

	TEST_METHOD(Test_sameTypeStmtNoParseWithClauses) {
		string query = "r.stmt#=3";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration readDec = Declaration(make_shared<DE::Read>(), "r");
		std::vector<Declaration> inputDecs = { readDec };

		struct SynAttr synAttrLhs = { readDec, ArgType::ATTR_REF_INT };
		struct Argument arg1 = { synAttrLhs };
		struct Argument arg2 = { Integer("3")};
		WithClause withCl = WithClause(arg1, arg2);
		struct WithClauses expectedWithCl = { {}, { withCl } };

		WithClauses withResult = WithParser::parse({ inputTokens }, inputDecs);
		Assert::AreEqual(expectedWithCl, withResult);
	}

	TEST_METHOD(Test_sameTypeValueParseWithClauses) {
		string query = "c.value=3";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration constDec = Declaration(make_shared<DE::Constant>(), "c");
		std::vector<Declaration> inputDecs = { constDec };

		struct SynAttr synAttr1 = { constDec, ArgType::ATTR_REF_INT };
		struct Argument arg1 = { synAttr1 };
		struct Argument arg2 = { Integer("3")};
		WithClause withCl = WithClause(arg1, arg2);
		struct WithClauses expectedWithCl = { {}, { withCl } };

		WithClauses withResult = WithParser::parse({ inputTokens }, inputDecs);
		Assert::AreEqual(expectedWithCl, withResult);
	}

	TEST_METHOD(Test_sameTypeProcNameParseWithClauses) {
		string query = "proc.procName=\"main\"";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration procDec = Declaration(make_shared<DE::Procedure>(), "proc");
		std::vector<Declaration> inputDecs = { procDec };
		struct SynAttr lhsSynAttr = { procDec, ArgType::ATTR_REF_NAME };
		struct Argument lhsArg = { lhsSynAttr };
		struct Argument rhsArg = { StringExp("main") };

		WithClause withCl = WithClause(lhsArg, rhsArg);
		struct WithClauses expectedWithCl = { {}, { withCl } };

		WithClauses withResult = WithParser::parse({ inputTokens }, inputDecs);
		Assert::AreEqual(expectedWithCl, withResult);
	}

	TEST_METHOD(Test_sameTypeCallProcNameParseWithClauses) {
		string query = "c1.procName=\"main\"";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration callDec = Declaration(make_shared<DE::Call>(), "c1");
		std::vector<Declaration> inputDecs = { callDec };
		struct SynAttr lhsSynAttr = { callDec, ArgType::ATTR_REF_NAME };
		struct Argument lhsArg = { lhsSynAttr };
		struct Argument rhsArg = { StringExp("main") };

		WithClause withCl = WithClause(lhsArg, rhsArg);
		struct WithClauses expectedWithCl = { {}, { withCl } };

		WithClauses withResult = WithParser::parse({ inputTokens }, inputDecs);
		Assert::AreEqual(expectedWithCl, withResult);
	}
	};
}