#include "stdafx.h"
#include "CppUnitTest.h"
#include <unordered_set>

#include "../source/PKB/InsertFacade.h"
#include "../source/PKB/GetFacade.h"
#include "../source/SourceProcessor/SPManager.h"
#include "../source/Request/GetRequests/GetRelationshipRequests/GetCallsRequest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


// Integration of Calls relationship extraction to pkb
// Test that extracted calls can be retrieved at pkb

using testStub = std::vector<std::pair<std::string, std::string>>;
using outputStub = std::unordered_set<std::string>;

struct CallsPkbHelper {
	outputStub outputConverter(RelationshipResults inp) {
		outputStub res{};
		for each (pair<std::string, std::string> var in inp) {
			res.insert(var.first + " " + var.second);
		}
		return res;
	}

	outputStub outputConverter(testStub inp) {
		outputStub res{};
		for each (pair<string, string> var in inp) {
			res.insert(var.first + " " + var.second);
		}
		return res;
	}
};
auto c = CallsPkbHelper();

namespace IntegrationTesting {
	TEST_CLASS(TestCallsToPKB) {
public:
	TEST_METHOD(TestNoCall) {
		auto pkb = PKB();
		auto insertFacade = InsertFacade(pkb);
		auto getFacade = GetFacade(pkb);
		auto source = "procedure a {read a; print b;}";
		SPManager::construct(source).populatePKB(insertFacade);

		auto request = std::make_shared<GetCallsRequest>(Mode::ALL);
		auto actual = getFacade.get(request);

		testStub expected{};

		Assert::IsTrue(c.outputConverter(expected) == c.outputConverter(actual));
	}

	TEST_METHOD(TestCallsSingle) {
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

		auto request = std::make_shared<GetCallsRequest>(Mode::ALL);
		auto actual = getFacade.get(request);

		testStub expected{ {"p","q"} };

		Assert::IsTrue(c.outputConverter(expected) == c.outputConverter(actual));
	}

	TEST_METHOD(TestCallsMultiple) {
		auto pkb = PKB();
		auto insertFacade = InsertFacade(pkb);
		auto getFacade = GetFacade(pkb);
		auto source = "procedure p {"
						"call q;"
						"call r;"
						"call s;"
					  "}"
					  "procedure q {"
						"read a;"
					  "}"
					  "procedure r {"
						"read a;"
					  "}"
					  "procedure s {"
						"read a;"
					  "}";
		SPManager::construct(source).populatePKB(insertFacade);

		auto request = std::make_shared<GetCallsRequest>(Mode::ALL);
		auto actual = getFacade.get(request);

		testStub expected{ {"p","q"}, {"p", "r"}, {"p", "s"} };

		Assert::IsTrue(c.outputConverter(expected) == c.outputConverter(actual));
	}
	};
}