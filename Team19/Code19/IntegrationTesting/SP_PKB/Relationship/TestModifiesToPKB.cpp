#include "stdafx.h"
#include "CppUnitTest.h"
#include <unordered_set>

#include "../source/PKB/InsertFacade.h"
#include "../source/PKB/GetFacade.h"
#include "../source/SourceProcessor/SPManager.h"
#include "../source/Request/GetRequests/GetRelationshipRequests/GetModifiesRequest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


// Integration of Modifies relationship extraction to pkb
// Test that extracted modifies can be retrieved at pkb

using testStubProcedure = std::vector<std::pair<std::string, std::string>>;
using testStubStatement = std::vector<std::pair<int, std::string>>;
using outputStub = std::unordered_set<std::string>;

struct ModifiesPkbHelper {
	outputStub outputConverter(RelationshipResults inp) {
		outputStub res{};
		for each (pair<std::string, std::string> var in inp) {
			res.insert(var.first + " " + var.second);
		}
		return res;
	}

	outputStub outputConverter(testStubProcedure p, testStubStatement s) {
		outputStub res{};
		for each (pair<string, string> var in p) {
			res.insert(var.first + " " + var.second);
		}
		for each (pair<int, string> var in s) {
			res.insert(to_string(var.first) + " " + var.second);
		}
		return res;
	}
};
auto m = ModifiesPkbHelper();

namespace IntegrationTesting {
	TEST_CLASS(TestModifiesToPKB) {
public:
	TEST_METHOD(TestNoModifies) {
		auto pkb = PKB();
		auto insertFacade = InsertFacade(pkb);
		auto getFacade = GetFacade(pkb);
		auto source = "procedure a {print b;}";
		SPManager::construct(source).populatePKB(insertFacade);

		auto request = std::make_shared<GetModifiesRequest>(Mode::ALL);
		auto actual = getFacade.get(request);

		testStubProcedure expectedP{};
		testStubStatement expectedS{};

		Assert::IsTrue(m.outputConverter(expectedP, expectedS) == m.outputConverter(actual));
	}

	TEST_METHOD(TestModifiesSingle) {
		auto pkb = PKB();
		auto insertFacade = InsertFacade(pkb);
		auto getFacade = GetFacade(pkb);
		auto source = "procedure a {read b;}";
		SPManager::construct(source).populatePKB(insertFacade);

		auto request = std::make_shared<GetModifiesRequest>(Mode::ALL);
		auto actual = getFacade.get(request);

		testStubProcedure expectedP{ {"a", "b"} };
		testStubStatement expectedS{ {1, "b"} };

		Assert::IsTrue(m.outputConverter(expectedP, expectedS) == m.outputConverter(actual));
	}

	TEST_METHOD(TestModifiesMultiple) {
		auto pkb = PKB();
		auto insertFacade = InsertFacade(pkb);
		auto getFacade = GetFacade(pkb);
		auto source = "procedure a {read b; c = d; read e;}";
		SPManager::construct(source).populatePKB(insertFacade);

		auto request = std::make_shared<GetModifiesRequest>(Mode::ALL);
		auto actual = getFacade.get(request);

		testStubProcedure expectedP{ {"a", "b"}, {"a", "c"}, {"a", "e"} };
		testStubStatement expectedS{ {1, "b"}, {2, "c"}, {3, "e"} };

		Assert::IsTrue(m.outputConverter(expectedP, expectedS) == m.outputConverter(actual));
	}

	TEST_METHOD(TestModifiesWhile) {
		auto pkb = PKB();
		auto insertFacade = InsertFacade(pkb);
		auto getFacade = GetFacade(pkb);
		auto source = "procedure p {"
						"while (a == a) {"
							"c = d;"
							"read e;"
						"}"
					  "}";
		SPManager::construct(source).populatePKB(insertFacade);

		auto request = std::make_shared<GetModifiesRequest>(Mode::ALL);
		auto actual = getFacade.get(request);

		testStubProcedure expectedP{ {"p", "c"}, {"p", "e"} };
		testStubStatement expectedS{ {1, "c"}, {1, "e"}, {2, "c"}, {3, "e"} };

		Assert::IsTrue(m.outputConverter(expectedP, expectedS) == m.outputConverter(actual));
	}

	TEST_METHOD(TestModifiesIf) {
		auto pkb = PKB();
		auto insertFacade = InsertFacade(pkb);
		auto getFacade = GetFacade(pkb);
		auto source = "procedure p {"
						"if (a == a) then {"
							"c = d;"
						"} else {"
							"read e;"
						"}"
					  "}";
		SPManager::construct(source).populatePKB(insertFacade);

		auto request = std::make_shared<GetModifiesRequest>(Mode::ALL);
		auto actual = getFacade.get(request);

		testStubProcedure expectedP{ {"p", "c"}, {"p", "e"} };
		testStubStatement expectedS{ {1, "c"}, {1, "e"}, {2, "c"}, {3, "e"} };

		Assert::IsTrue(m.outputConverter(expectedP, expectedS) == m.outputConverter(actual));
	}

	TEST_METHOD(TestModifiesCall) {
		auto pkb = PKB();
		auto insertFacade = InsertFacade(pkb);
		auto getFacade = GetFacade(pkb);
		auto source = "procedure p {"
						"call q;"
					  "}"
					  "procedure q {"
						"read a;"
					  "}";
		SPManager::construct(source).populatePKB(insertFacade);

		auto request = std::make_shared<GetModifiesRequest>(Mode::ALL);
		auto actual = getFacade.get(request);

		testStubProcedure expectedP{ {"p", "a"}, {"q", "a"} };
		testStubStatement expectedS{ {1, "a"}, {2, "a"} };

		Assert::IsTrue(m.outputConverter(expectedP, expectedS) == m.outputConverter(actual));
	}

	TEST_METHOD(TestModifiesIndirect) {
		auto pkb = PKB();
		auto insertFacade = InsertFacade(pkb);
		auto getFacade = GetFacade(pkb);
		auto source = "procedure p {"
						"while (a == a) {"
							"call q;"
						"}"
					  "}"
					  "procedure q {"
						"c = d;"
						"read e;"
						"call r;"
					  "}"
					  "procedure r {"
						"read f;"
					  "}";
		SPManager::construct(source).populatePKB(insertFacade);

		auto request = std::make_shared<GetModifiesRequest>(Mode::ALL);
		auto actual = getFacade.get(request);

		testStubProcedure expectedP{ {"p", "c"}, {"p", "e"}, {"p", "f"}, {"q", "c"}, {"q", "e"}, {"q", "f"}, {"r", "f"} };
		testStubStatement expectedS{
			{1, "c"},
			{1, "e"},
			{1, "f"},
			{2, "c"},
			{2, "e"},
			{2, "f"},
			{3, "c"},
			{4, "e"},
			{5, "f"},
			{6, "f"},
		};

		Assert::IsTrue(m.outputConverter(expectedP, expectedS) == m.outputConverter(actual));
	}
	};
}