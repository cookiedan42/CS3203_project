#include "stdafx.h"
#include "CppUnitTest.h"
#include <unordered_set>

#include "../source/PKB/InsertFacade.h"
#include "../source/PKB/GetFacade.h"
#include "../source/SourceProcessor/SPManager.h"
#include "../source/Request/GetRequests/GetRelationshipRequests/GetParentRequest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


// Integration of Parent relationship extraction to pkb
// Test that extracted parent can be retrieved at pkb

using testStub = std::vector<std::pair<int, int>>;
using outputStub = std::unordered_set<std::string>;

struct ParentPkbHelper {
	outputStub outputConverter(RelationshipResults inp) {
		outputStub res{};
		for each (pair<std::string, std::string> var in inp) {
			res.insert(var.first + " " + var.second);
		}
		return res;
	}

	outputStub outputConverter(testStub inp) {
		outputStub res{};
		for each (pair<int, int> var in inp) {
			res.insert(
				std::to_string(var.first)
				+ " " + std::to_string(var.second)
			);
		}
		return res;
	}
};
auto p = ParentPkbHelper();

namespace IntegrationTesting {
	TEST_CLASS(TestParentToPKB) {
public:
	TEST_METHOD(TestNoParent) {
		auto pkb = PKB();
		auto insertFacade = InsertFacade(pkb);
		auto getFacade = GetFacade(pkb);
		auto source = "procedure a {read a; print b;}";
		SPManager::construct(source).populatePKB(insertFacade);

		auto request = std::make_shared<GetParentRequest>(Mode::ALL);
		auto actual = getFacade.get(request);

		testStub expected{};

		Assert::IsTrue(p.outputConverter(expected) == p.outputConverter(actual));
	}

	TEST_METHOD(TestParentWhileSingle) {
		auto pkb = PKB();
		auto insertFacade = InsertFacade(pkb);
		auto getFacade = GetFacade(pkb);
		auto source = "procedure p {"
						"while (a == a) {"
							"read a;"
						"}"
					  "}";
		SPManager::construct(source).populatePKB(insertFacade);

		auto request = std::make_shared<GetParentRequest>(Mode::ALL);
		auto actual = getFacade.get(request);

		testStub expected{ {1,2} };

		Assert::IsTrue(p.outputConverter(expected) == p.outputConverter(actual));
	}

	TEST_METHOD(TestParentWhileMultiple) {
		auto pkb = PKB();
		auto insertFacade = InsertFacade(pkb);
		auto getFacade = GetFacade(pkb);
		auto source = "procedure a {"
						"while (a == a) {"
							"read a;"
							"print b;"
							"c = d * e;"
							"read f;"
							"print g;"
						"}"
					  "}";
		SPManager::construct(source).populatePKB(insertFacade);

		auto request = std::make_shared<GetParentRequest>(Mode::ALL);
		auto actual = getFacade.get(request);

		testStub expected{ {1,2}, {1,3}, {1,4}, {1,5}, {1,6} };

		Assert::IsTrue(p.outputConverter(expected) == p.outputConverter(actual));
	}

	TEST_METHOD(TestParentWhileNested) {
		auto pkb = PKB();
		auto insertFacade = InsertFacade(pkb);
		auto getFacade = GetFacade(pkb);
		auto source = "procedure p {"
						"while (a == a) {"
						"while (a == a) {"
						"while (a == a) {"
						"while (a == a) {"
						"read a;"
						"}}}}"
					  "}";
		SPManager::construct(source).populatePKB(insertFacade);

		auto request = std::make_shared<GetParentRequest>(Mode::ALL);
		auto actual = getFacade.get(request);

		testStub expected{ {1,2}, {2,3}, {3,4}, {4,5} };

		Assert::IsTrue(p.outputConverter(expected) == p.outputConverter(actual));
	}

	TEST_METHOD(TestParentIfSingle) {
		auto pkb = PKB();
		auto insertFacade = InsertFacade(pkb);
		auto getFacade = GetFacade(pkb);
		auto source = "procedure p {"
						"if (a == a) then {"
							"read a;"
						"} else {"
							"print b;"
						"}"
					  "}";
		SPManager::construct(source).populatePKB(insertFacade);

		auto request = std::make_shared<GetParentRequest>(Mode::ALL);
		auto actual = getFacade.get(request);

		testStub expected{ {1,2}, {1,3} };

		Assert::IsTrue(p.outputConverter(expected) == p.outputConverter(actual));
	}

	TEST_METHOD(TestParentIfMultiple) {
		auto pkb = PKB();
		auto insertFacade = InsertFacade(pkb);
		auto getFacade = GetFacade(pkb);
		auto source = "procedure p {"
						"if (a == a) then {"
							"read a;"
							"print b;"
							"c = d * e;"
						"} else {"
							"read f;"
							"print g;"
							"h = i *j;"
						"}"
					  "}";
		SPManager::construct(source).populatePKB(insertFacade);

		auto request = std::make_shared<GetParentRequest>(Mode::ALL);
		auto actual = getFacade.get(request);

		testStub expected{ {1,2}, {1,3}, {1,4}, {1,5}, {1,6}, {1,7} };

		Assert::IsTrue(p.outputConverter(expected) == p.outputConverter(actual));
	}

	TEST_METHOD(TestParentIfNested) {
		auto pkb = PKB();
		auto insertFacade = InsertFacade(pkb);
		auto getFacade = GetFacade(pkb);
		auto source = "procedure p {"
						"if (a == a) then {"
							"if (a == a) then {"
								"if (a == a) then {"
									"read a;"
								"} else {"
									"print b;"
								"}"
							"} else {"
								"print g;"
							"}"
						"} else {"
							"h = i *j;"
						"}"
					  "}";
		SPManager::construct(source).populatePKB(insertFacade);

		auto request = std::make_shared<GetParentRequest>(Mode::ALL);
		auto actual = getFacade.get(request);

		testStub expected{ {1,2}, {1,7}, {2,3}, {2,6}, {3,4}, {3,5} };

		Assert::IsTrue(p.outputConverter(expected) == p.outputConverter(actual));
	}
	};
}