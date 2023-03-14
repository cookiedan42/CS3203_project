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
			auto declarations_converter = std::wstring_convert<std::codecvt_utf8<wchar_t>>();

			static std::wstring printDeclaration(const Declaration& dec) {
				std::string result = "Declaration " + dec.synType->type + " with " + dec.synName;
				return declarations_converter.from_bytes(result);
			}

			static std::wstring printDeclarations(const std::vector<Declaration>& decList) {
				std::string result = "Declarations list containing declarations: ";
				for (auto dec : decList) {
					std::wstring decStr = printDeclaration(dec);
					result += declarations_converter.to_bytes(decStr) + ";";
				}
				return declarations_converter.from_bytes(result);
			}

			template<>
			static std::wstring ToString<std::vector<Declaration>>(const std::vector<Declaration>& decList) {
				return printDeclarations(decList);
			}
		}
	}
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestQPDeclarationsParser) {
public:

	/* Single elem - attr */
	TEST_METHOD(Test_validDeclarationOneSyn) {
		string query = "assign a; Select a";
		auto tokens = QuerySplitter::tokenize(query);
		auto qs = QuerySplitter(tokens);
		qs.parse();
		
		Declaration keyDec = Declaration(make_shared<DE::Assign>(), "a");
		std::vector<Declaration> expectedDecs = { keyDec };

		std::vector<Declaration> returnedDecs = DeclarationsParser::parse(qs.getDeclarations());
		Assert::AreEqual(expectedDecs, returnedDecs);
	}

	TEST_METHOD(Test_validDeclarationOneDecMultSyn) {
		string query = "assign a, b, c; Select a";
		auto tokens = QuerySplitter::tokenize(query);
		auto qs = QuerySplitter(tokens);
		qs.parse();

		Declaration firstDec = Declaration(make_shared<DE::Assign>(), "a");
		Declaration secondDec = Declaration(make_shared<DE::Assign>(), "b");
		Declaration thirdDec = Declaration(make_shared<DE::Assign>(), "c");
		std::vector<Declaration> expectedDecs = { firstDec, secondDec, thirdDec };

		std::vector<Declaration> returnedDecs = DeclarationsParser::parse(qs.getDeclarations());
		Assert::AreEqual(expectedDecs, returnedDecs);
	}

	TEST_METHOD(Test_validDeclarationMultDecMultSyn) {
		string query = "assign a; variable x, y; Select a";
		auto tokens = QuerySplitter::tokenize(query);
		auto qs = QuerySplitter(tokens);
		qs.parse();

		Declaration assignDec = Declaration(make_shared<DE::Assign>(), "a");
		Declaration xDec = Declaration(make_shared<DE::Variable>(), "x");
		Declaration yDec = Declaration(make_shared<DE::Variable>(), "y");
		std::vector<Declaration> expectedDecs = { assignDec, xDec, yDec };

		std::vector<Declaration> returnedDecs = DeclarationsParser::parse(qs.getDeclarations());
		Assert::AreEqual(expectedDecs, returnedDecs);
	}

	TEST_METHOD(Test_validDeclarationUsingKeyword) {
		string query = "assign assign; Select assign";
		auto tokens = QuerySplitter::tokenize(query);
		auto qs = QuerySplitter(tokens);
		qs.parse();

		Declaration assignDec = Declaration(make_shared<DE::Assign>(), "assign");
		std::vector<Declaration> expectedDecs = { assignDec };

		std::vector<Declaration> returnedDecs = DeclarationsParser::parse(qs.getDeclarations());
		Assert::AreEqual(expectedDecs, returnedDecs);
	}

	TEST_METHOD(Test_invalidDeclarationRepeatedSynNameMultDec) {
		string query = "stmt such; variable such; Select such such that Next()";
		auto tokens = QuerySplitter::tokenize(query);
		auto qs = QuerySplitter(tokens);
		qs.parse();

		Assert::ExpectException<QuerySemanticException>([&]() {
			std::vector<Declaration> returnedDecs = DeclarationsParser::parse(qs.getDeclarations());
			});
	}

	TEST_METHOD(Test_invalidDeclarationRepeatedSynNameMultSynOneDec) {
		string query = "stmt such, such; Select such such that Next()";
		auto tokens = QuerySplitter::tokenize(query);
		auto qs = QuerySplitter(tokens);
		qs.parse();

		Assert::ExpectException<QuerySemanticException>([&]() {
			std::vector<Declaration> returnedDecs = DeclarationsParser::parse(qs.getDeclarations());
			});
	}

	};
}