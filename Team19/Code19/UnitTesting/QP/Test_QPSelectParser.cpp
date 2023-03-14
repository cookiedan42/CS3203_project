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
			auto select_converter = std::wstring_convert<std::codecvt_utf8<wchar_t>>();

			static std::string printSelectDec(const Declaration& dec) {
				std::string result = "Declaration " + dec.synType->type + " with " + dec.synName;
				return result;
			}

			static std::wstring printSelectPair(const SelectObject::selectPair& selectPr) {
				std::string result = printSelectDec(selectPr.first);
				if (selectPr.second == ArgType::ATTR_REF_INT) {
					result += ", Type: ATTR_REF_INT";
				}
				else if (selectPr.second == ArgType::ATTR_REF_NAME) {
					result += ", Type: ATTR_REF_NAME";
				}
				else if (selectPr.second == ArgType::SYN) {
					result += ", just a syn";
				}
				return select_converter.from_bytes(result);
			}

			static std::wstring printSelectObject(const SelectObject& selectObj) {
				std::string result = "SelectObject containing selectPairs: ";
				for (auto pair : selectObj.selectPairs) {
					std::wstring selectPairStr = printSelectPair(pair);
					result += select_converter.to_bytes(selectPairStr) + ";";
				}
				return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(result);
			}

			template<>
			static std::wstring ToString<SelectObject>(const SelectObject& selectObj) {
				return printSelectObject(selectObj);
			}
		}
	}
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestQPSelectParser) {
public:

	/* Single elem - attr */
	TEST_METHOD(Test_validSelectStatementStmtNoAttr) {
		string query = "s.stmt#";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration keyDec = Declaration(make_shared<DE::Statement>(), "s");
		std::vector<Declaration> inputDecs = { keyDec };
		SelectObject expectedSelectObj = SelectObject({ make_pair(keyDec, ArgType::ATTR_REF_INT) });

		SelectObject selectResult = SelectParser::parse(inputTokens, inputDecs);
		Assert::AreEqual(expectedSelectObj, selectResult);
	}

	TEST_METHOD(Test_validSelectReadStmtNoAttr) {
		string query = "r.stmt#";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration keyDec = Declaration(make_shared<DE::Read>(), "r");
		std::vector<Declaration> inputDecs = { keyDec };
		SelectObject expectedSelectObj = SelectObject({ make_pair(keyDec, ArgType::ATTR_REF_INT) });

		SelectObject selectResult = SelectParser::parse(inputTokens, inputDecs);
		Assert::AreEqual(expectedSelectObj, selectResult);
	}

	TEST_METHOD(Test_validSelectPrintStmtNoAttr) {
		string query = "p.stmt#";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration keyDec = Declaration(make_shared<DE::Print>(), "p");
		std::vector<Declaration> inputDecs = { keyDec };
		SelectObject expectedSelectObj = SelectObject({ make_pair(keyDec, ArgType::ATTR_REF_INT) });

		SelectObject selectResult = SelectParser::parse(inputTokens, inputDecs);
		Assert::AreEqual(expectedSelectObj, selectResult);
	}

	TEST_METHOD(Test_validSelectCallStmtNoAttr) {
		string query = "c1.stmt#";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration keyDec = Declaration(make_shared<DE::Call>(), "c1");
		std::vector<Declaration> inputDecs = { keyDec };
		SelectObject expectedSelectObj = SelectObject({ make_pair(keyDec, ArgType::ATTR_REF_INT) });

		SelectObject selectResult = SelectParser::parse(inputTokens, inputDecs);
		Assert::AreEqual(expectedSelectObj, selectResult);
	}

	TEST_METHOD(Test_validSelectWhileStmtNoAttr) {
		string query = "wh.stmt#";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration keyDec = Declaration(make_shared<DE::While>(), "wh");
		std::vector<Declaration> inputDecs = { keyDec };
		SelectObject expectedSelectObj = SelectObject({ make_pair(keyDec, ArgType::ATTR_REF_INT) });

		SelectObject selectResult = SelectParser::parse(inputTokens, inputDecs);
		Assert::AreEqual(expectedSelectObj, selectResult);
	}

	TEST_METHOD(Test_validSelectIfStmtNoAttr) {
		string query = "if1.stmt#";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration keyDec = Declaration(make_shared<DE::If>(), "if1");
		std::vector<Declaration> inputDecs = { keyDec };
		SelectObject expectedSelectObj = SelectObject({ make_pair(keyDec, ArgType::ATTR_REF_INT) });

		SelectObject selectResult = SelectParser::parse(inputTokens, inputDecs);
		Assert::AreEqual(expectedSelectObj, selectResult);
	}

	TEST_METHOD(Test_validSelectAssignStmtNoAttr) {
		string query = "a123a.stmt#";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration keyDec = Declaration(make_shared<DE::Assign>(), "a123a");
		std::vector<Declaration> inputDecs = { keyDec };
		SelectObject expectedSelectObj = SelectObject({ make_pair(keyDec, ArgType::ATTR_REF_INT) });

		SelectObject selectResult = SelectParser::parse(inputTokens, inputDecs);
		Assert::AreEqual(expectedSelectObj, selectResult);
	}

	TEST_METHOD(Test_validSelectConstantValueAttr) {
		string query = "const1.value";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration keyDec = Declaration(make_shared<DE::Constant>(), "const1");
		std::vector<Declaration> inputDecs = { keyDec };
		SelectObject expectedSelectObj = SelectObject({ make_pair(keyDec, ArgType::ATTR_REF_INT) });

		SelectObject selectResult = SelectParser::parse(inputTokens, inputDecs);
		Assert::AreEqual(expectedSelectObj, selectResult);
	}

	TEST_METHOD(Test_validSelectProcedureProcNameAttr) {
		string query = "proc.procName";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration keyDec = Declaration(make_shared<DE::Procedure>(), "proc");
		std::vector<Declaration> inputDecs = { keyDec };
		SelectObject expectedSelectObj = SelectObject({ make_pair(keyDec, ArgType::ATTR_REF_NAME) });

		SelectObject selectResult = SelectParser::parse(inputTokens, inputDecs);
		Assert::AreEqual(expectedSelectObj, selectResult);
	}

	TEST_METHOD(Test_validSelectCallProcNameAttr) {
		string query = "c1.procName";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration keyDec = Declaration(make_shared<DE::Call>(), "c1");
		std::vector<Declaration> inputDecs = { keyDec };
		SelectObject expectedSelectObj = SelectObject({ make_pair(keyDec, ArgType::ATTR_REF_NAME) });

		SelectObject selectResult = SelectParser::parse(inputTokens, inputDecs);
		Assert::AreEqual(expectedSelectObj, selectResult);
	}

	TEST_METHOD(Test_validSelectVariableVarNameAttr) {
		string query = "var.varName";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration keyDec = Declaration(make_shared<DE::Variable>(), "var");
		std::vector<Declaration> inputDecs = { keyDec };
		SelectObject expectedSelectObj = SelectObject({ make_pair(keyDec, ArgType::ATTR_REF_NAME) });

		SelectObject selectResult = SelectParser::parse(inputTokens, inputDecs);
		Assert::AreEqual(expectedSelectObj, selectResult);
	}

	TEST_METHOD(Test_validSelectReadVarNameAttr) {
		string query = "r1.varName";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration keyDec = Declaration(make_shared<DE::Read>(), "r1");
		std::vector<Declaration> inputDecs = { keyDec };
		SelectObject expectedSelectObj = SelectObject({ make_pair(keyDec, ArgType::ATTR_REF_NAME) });

		SelectObject selectResult = SelectParser::parse(inputTokens, inputDecs);
		Assert::AreEqual(expectedSelectObj, selectResult);
	}

	TEST_METHOD(Test_validSelectPrintVarNameAttr) {
		string query = "print.varName";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration keyDec = Declaration(make_shared<DE::Print>(), "print");
		std::vector<Declaration> inputDecs = { keyDec };
		SelectObject expectedSelectObj = SelectObject({ make_pair(keyDec, ArgType::ATTR_REF_NAME) });

		SelectObject selectResult = SelectParser::parse(inputTokens, inputDecs);
		Assert::AreEqual(expectedSelectObj, selectResult);
	}

	TEST_METHOD(Test_invalidSelectInvalidVarName) {
		string query = "print.varName";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration keyDec = Declaration(make_shared<DE::Statement>(), "s");
		std::vector<Declaration> inputDecs = { keyDec };

		Assert::ExpectException<QuerySyntaxException>([&]() {
			SelectObject selectResult = SelectParser::parse(inputTokens, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidSelectInvalidAttrName) {
		string query = "var.variableName";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration keyDec = Declaration(make_shared<DE::Variable>(), "var");
		std::vector<Declaration> inputDecs = { keyDec };

		Assert::ExpectException<QuerySyntaxException>([&]() {
			SelectObject selectResult = SelectParser::parse(inputTokens, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidSelectMultAttrDelim) {
		string query = "read.variable.name";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration keyDec = Declaration(make_shared<DE::Read>(), "read");
		std::vector<Declaration> inputDecs = { keyDec };

		Assert::ExpectException<QuerySyntaxException>([&]() {
			SelectObject selectResult = SelectParser::parse(inputTokens, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidSelectMissingAttrDelim) {
		string query = "stmtstmt#";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration keyDec = Declaration(make_shared<DE::Statement>(), "stmt");
		std::vector<Declaration> inputDecs = { keyDec };

		Assert::ExpectException<QuerySemanticException>([&]() {
			SelectObject selectResult = SelectParser::parse(inputTokens, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidSelectOnlyAttrName) {
		string query = "procName";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration keyDec = Declaration(make_shared<DE::Statement>(), "stmt");
		std::vector<Declaration> inputDecs = { keyDec };

		Assert::ExpectException<QuerySemanticException>([&]() {
			SelectObject selectResult = SelectParser::parse(inputTokens, inputDecs);
			});
	}

	/* Single elem - synonym */
	TEST_METHOD(Test_validSelectVariableSyn) {
		string query = "var";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration keyDec = Declaration(make_shared<DE::Variable>(), "var");
		std::vector<Declaration> inputDecs = { keyDec };
		SelectObject expectedSelectObj = SelectObject({ make_pair(keyDec, ArgType::SYN) });

		SelectObject selectResult = SelectParser::parse(inputTokens, inputDecs);
		Assert::AreEqual(expectedSelectObj, selectResult);
	}

	TEST_METHOD(Test_validSelectBooleanSyn) {
		string query = "BOOLEAN";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration varDec = Declaration(make_shared<DE::Variable>(), "var");
		Declaration boolDec = Declaration(make_shared<DE::Statement>(), "BOOLEAN");
		std::vector<Declaration> inputDecs = { varDec, boolDec };
		SelectObject expectedSelectObj = SelectObject({ make_pair(boolDec, ArgType::SYN) });

		SelectObject selectResult = SelectParser::parse(inputTokens, inputDecs);
		Assert::AreEqual(expectedSelectObj, selectResult);
	}

	TEST_METHOD(Test_validSelectSelectProcSyn) {
		string query = "Select";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration keyDec = Declaration(make_shared<DE::Procedure>(), "Select");
		std::vector<Declaration> inputDecs = { keyDec };
		SelectObject expectedSelectObj = SelectObject({ make_pair(keyDec, ArgType::SYN) });

		SelectObject selectResult = SelectParser::parse(inputTokens, inputDecs);
		Assert::AreEqual(expectedSelectObj, selectResult);
	}

	TEST_METHOD(Test_invalidSelectSynNotInDec) {
		string query = "b";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration keyDec = Declaration(make_shared<DE::Assign>(), "a");
		std::vector<Declaration> inputDecs = { keyDec };

		Assert::ExpectException<QuerySemanticException>([&]() {
			SelectObject selectResult = SelectParser::parse(inputTokens, inputDecs);
			});
	}

	/* Single elem - boolean */
	TEST_METHOD(Test_validSelectBoolean) {
		string query = "BOOLEAN";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		std::vector<Declaration> inputDecs = {};
		SelectObject expectedSelectObj = SelectObject();

		SelectObject selectResult = SelectParser::parse(inputTokens, inputDecs);
		Assert::AreEqual(expectedSelectObj, selectResult);
	}

	/* Tuples */
	TEST_METHOD(Test_validSelectTupleOfSyns) {
		string query = "<var, c1, c2>";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration varDec = Declaration(make_shared<DE::Variable>(), "var");
		Declaration constDec1 = Declaration(make_shared<DE::Constant>(), "c1");
		Declaration constDec2 = Declaration(make_shared<DE::Constant>(), "c2");
		std::vector<Declaration> inputDecs = { varDec, constDec1, constDec2 };
		SelectObject expectedSelectObj = SelectObject({ 
			make_pair(varDec, ArgType::SYN),
			make_pair(constDec1, ArgType::SYN),
			make_pair(constDec2, ArgType::SYN),
		});

		SelectObject selectResult = SelectParser::parse(inputTokens, inputDecs);
		Assert::AreEqual(expectedSelectObj, selectResult);
	}

	TEST_METHOD(Test_validSelectTupleOfAttr) {
		string query = "<var.varName, const.value, call.procName, pr.stmt#>";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration varDec = Declaration(make_shared<DE::Variable>(), "var");
		Declaration constDec = Declaration(make_shared<DE::Constant>(), "const");
		Declaration callDec = Declaration(make_shared<DE::Call>(), "call");
		Declaration printDec = Declaration(make_shared<DE::Print>(), "pr");
		std::vector<Declaration> inputDecs = { varDec, constDec, callDec, printDec };
		SelectObject expectedSelectObj = SelectObject({
			make_pair(varDec, ArgType::ATTR_REF_NAME),
			make_pair(constDec, ArgType::ATTR_REF_INT),
			make_pair(callDec, ArgType::ATTR_REF_NAME),
			make_pair(printDec, ArgType::ATTR_REF_INT),
			});

		SelectObject selectResult = SelectParser::parse(inputTokens, inputDecs);
		Assert::AreEqual(expectedSelectObj, selectResult);
	}

	TEST_METHOD(Test_validSelectTupleMix) {
		string query = "<var.varName, call.stmt#, BOOLEAN>";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration assignDec = Declaration(make_shared<DE::Variable>(), "var");
		Declaration boolDec = Declaration(make_shared<DE::Constant>(), "BOOLEAN");
		Declaration callDec = Declaration(make_shared<DE::Call>(), "call");
		std::vector<Declaration> inputDecs = { assignDec, boolDec, callDec};
		SelectObject expectedSelectObj = SelectObject({
			make_pair(assignDec, ArgType::ATTR_REF_NAME),
			make_pair(callDec, ArgType::ATTR_REF_INT),
			make_pair(boolDec, ArgType::SYN),
			});

		SelectObject selectResult = SelectParser::parse(inputTokens, inputDecs);
		Assert::AreEqual(expectedSelectObj, selectResult);
	}

	TEST_METHOD(Test_invalidSelectTupleMixInvalidSyn) {
		string query = "<a, b.value, d>";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration assignDec = Declaration(make_shared<DE::Assign>(), "a");
		Declaration constDec = Declaration(make_shared<DE::Constant>(), "b");
		Declaration readDec = Declaration(make_shared<DE::Read>(), "c");
		std::vector<Declaration> inputDecs = { assignDec, constDec, readDec };

		Assert::ExpectException<QuerySemanticException>([&]() {
			SelectObject selectResult = SelectParser::parse(inputTokens, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidSelectTupleMixIncorrectAttrName) {
		string query = "<a, b.varName, c>";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration assignDec = Declaration(make_shared<DE::Assign>(), "a");
		Declaration constDec = Declaration(make_shared<DE::Constant>(), "b");
		Declaration readDec = Declaration(make_shared<DE::Read>(), "c");
		std::vector<Declaration> inputDecs = { assignDec, constDec, readDec };

		Assert::ExpectException<QuerySyntaxException>([&]() {
			SelectObject selectResult = SelectParser::parse(inputTokens, inputDecs);
			});
	}

	TEST_METHOD(Test_invalidSelectTupleMixInvalidSynAttrName) {
		string query = "<a, d.stmt#, c>";
		std::vector<Token> inputTokens = TokenSplitter(query).tokenize();
		Declaration assignDec = Declaration(make_shared<DE::Assign>(), "a");
		Declaration constDec = Declaration(make_shared<DE::Constant>(), "b");
		Declaration readDec = Declaration(make_shared<DE::Read>(), "c");
		std::vector<Declaration> inputDecs = { assignDec, constDec, readDec };

		Assert::ExpectException<QuerySyntaxException>([&]() {
			SelectObject selectResult = SelectParser::parse(inputTokens, inputDecs);
			});
	}

	};
}