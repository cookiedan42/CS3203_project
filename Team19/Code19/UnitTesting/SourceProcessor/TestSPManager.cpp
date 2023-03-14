#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/SourceProcessor/SPManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestSPManager) {
public:
	TEST_METHOD(Test_construct) {
	
		auto source = "procedure name {read a; read a;}";
		auto spm = SPManager::construct(source);
		auto pkb = PKB();
		auto insertFacade = InsertFacade(pkb);
		auto f = spm.de.getRelationshipRequests();
	
		Assert::AreEqual(
			size_t(5),
			f.size()
		);
	}
};
}