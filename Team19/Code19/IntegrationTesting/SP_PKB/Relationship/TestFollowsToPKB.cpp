#include "stdafx.h"
#include "CppUnitTest.h"
#include <unordered_set>

#include "../source/PKB/InsertFacade.h"
#include "../source/PKB/GetFacade.h"
#include "../source/SourceProcessor/SPManager.h"
#include "../source/Request/GetRequests/GetRelationshipRequests/GetFollowsRequest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


// Integration of Follows relationship extraction to pkb
// Test that extracted follows can be retrieved at pkb

using testStub = std::vector<std::pair<int, int>>;
using outputStub = std::unordered_set<std::string>;

struct FollowsPkbHelper {
	outputStub outputConverter(RelationshipResults inp) {
		outputStub res {};
		for each (pair<std::string,std::string> var in inp) {
			res.insert(var.first + " " + var.second);
		}
		return res;
	}

	outputStub outputConverter(testStub inp) {
		outputStub res {};
		for each (pair<int, int> var in inp) {
			res.insert(
				std::to_string(var.first) 
				+ " " + std::to_string(var.second)
			);
		}
		return res;
	}
};
auto fp = FollowsPkbHelper();

namespace IntegrationTesting {
	TEST_CLASS(TestFollowsToPKB) {
public:
	TEST_METHOD(TestNoFollows) {
		auto pkb = PKB();
		auto insertFacade = InsertFacade(pkb);
		auto getFacade = GetFacade(pkb);
		auto source = "procedure a {read a;}";
		SPManager::construct(source).populatePKB(insertFacade);

		auto request = std::make_shared<GetFollowsRequest>(Mode::ALL);
		auto actual = getFacade.get(request);

		testStub expected{  };

		Assert::IsTrue(fp.outputConverter(expected) == fp.outputConverter(actual));
	}

	TEST_METHOD(TestFollowsSingle) {
		auto pkb = PKB();
		auto insertFacade = InsertFacade(pkb);
		auto getFacade = GetFacade(pkb);
		auto source = "procedure a {read a;read a;}";
		SPManager::construct(source).populatePKB(insertFacade);
		
		auto request = std::make_shared<GetFollowsRequest>(Mode::ALL);
		auto actual = getFacade.get(request);

		testStub expected { {1,2} };

		Assert::IsTrue(fp.outputConverter(expected) == fp.outputConverter(actual));
	}

	TEST_METHOD(TestFollowsMultiple) {
		auto pkb = PKB();
		auto insertFacade = InsertFacade(pkb);
		auto getFacade = GetFacade(pkb);
		auto source = "procedure a {"
						"read a;"
						"print b;"
						"c = d * e;"
						"read f;"
						"print g;"
					  "}";
		SPManager::construct(source).populatePKB(insertFacade);

		auto request = std::make_shared<GetFollowsRequest>(Mode::ALL);
		auto actual = getFacade.get(request);

		testStub expected{ {1,2}, {2,3}, {3,4}, {4,5} };

		Assert::IsTrue(fp.outputConverter(expected) == fp.outputConverter(actual));
	}

	TEST_METHOD(TestFollowsFromWhile) {
		auto pkb = PKB();
		auto insertFacade = InsertFacade(pkb);
		auto getFacade = GetFacade(pkb);
		auto source = "procedure p {"
						"while (a == a) {"
							"read a;"
						"}"
						"print b;"
				   	  "}";
		SPManager::construct(source).populatePKB(insertFacade);

		auto request = std::make_shared<GetFollowsRequest>(Mode::ALL);
		auto actual = getFacade.get(request);

		testStub expected{ {1,3} };

		Assert::IsTrue(fp.outputConverter(expected) == fp.outputConverter(actual));
	}

	TEST_METHOD(TestFollowsNestedWhile) {
		auto pkb = PKB();
		auto insertFacade = InsertFacade(pkb);
		auto getFacade = GetFacade(pkb);
		auto source = "procedure p {"
						"while (a == a) {"
							"read a;"
							"print b;"
						"}"
					  "}";
		SPManager::construct(source).populatePKB(insertFacade);

		auto request = std::make_shared<GetFollowsRequest>(Mode::ALL);
		auto actual = getFacade.get(request);

		testStub expected{ {2,3} };

		Assert::IsTrue(fp.outputConverter(expected) == fp.outputConverter(actual));
	}

	TEST_METHOD(TestFollowsFromIf) {
		auto pkb = PKB();
		auto insertFacade = InsertFacade(pkb);
		auto getFacade = GetFacade(pkb);
		auto source = "procedure p {"
						"if (a == a) then {"
							"read a;"
						"} else {"
							"print b;"
						"}"
						"c = d;"
					  "}";
		SPManager::construct(source).populatePKB(insertFacade);

		auto request = std::make_shared<GetFollowsRequest>(Mode::ALL);
		auto actual = getFacade.get(request);

		testStub expected{ {1,4} };

		Assert::IsTrue(fp.outputConverter(expected) == fp.outputConverter(actual));
	}

	TEST_METHOD(TestFollowsNestedIf) {
		auto pkb = PKB();
		auto insertFacade = InsertFacade(pkb);
		auto getFacade = GetFacade(pkb);
		auto source = "procedure p {"
						"if (a == a) then {"
							"read a;"
							"c = d;"
						"} else {"
							"print b;"
							"e = f + 1;"
						"}"
					  "}";
		SPManager::construct(source).populatePKB(insertFacade);

		auto request = std::make_shared<GetFollowsRequest>(Mode::ALL);
		auto actual = getFacade.get(request);

		testStub expected{ {2,3}, {4,5} };

		Assert::IsTrue(fp.outputConverter(expected) == fp.outputConverter(actual));
	}
	};
}