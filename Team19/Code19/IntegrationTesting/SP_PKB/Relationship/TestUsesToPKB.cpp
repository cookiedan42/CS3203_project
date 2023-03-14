#include "stdafx.h"
#include "CppUnitTest.h"
#include <unordered_set>

#include "../source/PKB/InsertFacade.h"
#include "../source/PKB/GetFacade.h"
#include "../source/SourceProcessor/SPManager.h"
#include "../source/Request/GetRequests/GetRelationshipRequests/GetUsesRequest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


// Integration of Uses relationship extraction to pkb
// Test that extracted uses can be retrieved at pkb

using testStubProcedure = std::vector<std::pair<std::string, std::string>>;
using testStubStatement = std::vector<std::pair<int, std::string>>;
using outputStub = std::unordered_set<std::string>;

struct UsesPkbHelper {
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
auto u = UsesPkbHelper();

namespace IntegrationTesting {
	TEST_CLASS(TestUsesToPKB) {
public:
	TEST_METHOD(TestNoUses) {
		auto pkb = PKB();
		auto insertFacade = InsertFacade(pkb);
		auto getFacade = GetFacade(pkb);
		auto source = "procedure a {read b;}";
		SPManager::construct(source).populatePKB(insertFacade);

		auto request = std::make_shared<GetUsesRequest>(Mode::ALL);
		auto actual = getFacade.get(request);

		testStubProcedure expectedP{};
		testStubStatement expectedS{};

		Assert::IsTrue(u.outputConverter(expectedP, expectedS) == u.outputConverter(actual));
	}

	TEST_METHOD(TestUsesSingle) {
		auto pkb = PKB();
		auto insertFacade = InsertFacade(pkb);
		auto getFacade = GetFacade(pkb);
		auto source = "procedure a {print b;}";
		SPManager::construct(source).populatePKB(insertFacade);

		auto request = std::make_shared<GetUsesRequest>(Mode::ALL);
		auto actual = getFacade.get(request);

		testStubProcedure expectedP{ {"a", "b"} };
		testStubStatement expectedS{ {1, "b"} };

		Assert::IsTrue(u.outputConverter(expectedP, expectedS) == u.outputConverter(actual));
	}

	TEST_METHOD(TestUsesMultiple) {
		auto pkb = PKB();
		auto insertFacade = InsertFacade(pkb);
		auto getFacade = GetFacade(pkb);
		auto source = "procedure a {print b; c = d + e * f; print g;}";
		SPManager::construct(source).populatePKB(insertFacade);

		auto request = std::make_shared<GetUsesRequest>(Mode::ALL);
		auto actual = getFacade.get(request);

		testStubProcedure expectedP{ {"a", "b"}, {"a", "d"}, {"a", "e"}, {"a", "f"}, {"a", "g"} };
		testStubStatement expectedS{ {1, "b"}, {2, "d"}, {2, "e"}, {2, "f"}, {3, "g"} };

		Assert::IsTrue(u.outputConverter(expectedP, expectedS) == u.outputConverter(actual));
	}

	TEST_METHOD(TestUsesWhile) {
		auto pkb = PKB();
		auto insertFacade = InsertFacade(pkb);
		auto getFacade = GetFacade(pkb);
		auto source = "procedure p {"
						"while (a == a) {"
							"c = d;"
							"print e;"
						"}"
					  "}";
		SPManager::construct(source).populatePKB(insertFacade);

		auto request = std::make_shared<GetUsesRequest>(Mode::ALL);
		auto actual = getFacade.get(request);

		testStubProcedure expectedP{ {"p", "a"}, {"p", "d"}, {"p", "e"} };
		testStubStatement expectedS{ {1, "a"}, {1, "d"}, {1, "e"}, {2, "d"}, {3, "e"} };

		Assert::IsTrue(u.outputConverter(expectedP, expectedS) == u.outputConverter(actual));
	}

	TEST_METHOD(TestUsesIf) {
		auto pkb = PKB();
		auto insertFacade = InsertFacade(pkb);
		auto getFacade = GetFacade(pkb);
		auto source = "procedure p {"
						"if (a == a) then {"
							"c = d;"
						"} else {"
							"print e;"
						"}"
					  "}";
		SPManager::construct(source).populatePKB(insertFacade);

		auto request = std::make_shared<GetUsesRequest>(Mode::ALL);
		auto actual = getFacade.get(request);

		testStubProcedure expectedP{ {"p", "a"}, {"p", "d"}, {"p", "e"} };
		testStubStatement expectedS{ {1, "a"}, {1, "d"}, {1, "e"}, {2, "d"}, {3, "e"} };

		Assert::IsTrue(u.outputConverter(expectedP, expectedS) == u.outputConverter(actual));
	}

	TEST_METHOD(TestUsesCall) {
		auto pkb = PKB();
		auto insertFacade = InsertFacade(pkb);
		auto getFacade = GetFacade(pkb);
		auto source = "procedure p {"
						"call q;"
					  "}"
					  "procedure q {"
						"print a;"
					  "}";
		SPManager::construct(source).populatePKB(insertFacade);

		auto request = std::make_shared<GetUsesRequest>(Mode::ALL);
		auto actual = getFacade.get(request);

		testStubProcedure expectedP{ {"p", "a"}, {"q", "a"} };
		testStubStatement expectedS{ {1, "a"}, {2, "a"} };

		Assert::IsTrue(u.outputConverter(expectedP, expectedS) == u.outputConverter(actual));
	}

	TEST_METHOD(TestUsesIndirect) {
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
			"print e;"
			"call r;"
			"}"
			"procedure r {"
			"print f;"
			"}";
		SPManager::construct(source).populatePKB(insertFacade);

		auto request = std::make_shared<GetUsesRequest>(Mode::ALL);
		auto actual = getFacade.get(request);

		testStubProcedure expectedP{
			{"p", "a"},
			{"p", "d"},
			{"p", "e"},
			{"p", "f"},
			{"q", "d"},
			{"q", "e"},
			{"q", "f"},
			{"r", "f"},
		};
		testStubStatement expectedS{
			{1, "a"},
			{1, "d"},
			{1, "e"},
			{1, "f"},
			{2, "d"},
			{2, "e"},
			{2, "f"},
			{3, "d"},
			{4, "e"},
			{5, "f"},
			{6, "f"},
		};

		Assert::IsTrue(u.outputConverter(expectedP, expectedS) == u.outputConverter(actual));
	}
	};
}