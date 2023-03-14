#include "stdafx.h"
#include "CppUnitTest.h"
#include <unordered_set>
#include "../AffectsContext.h"
#include "../source/QP/QueryEvaluator/AffectsEvaluator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace Test_QE_Affects {
	TEST_CLASS(Test_QEAffects_calls) {


		const Context callModified = Context(
			"procedure proc1 {a=1;call proc2; b=a;}"
			"procedure proc2 {a=1;}"
		);

		const Context callNotModified = Context(
			"procedure proc1 {a=1;call proc2; b=a;}"
			"procedure proc2 {b=1;}"
		);
		shared_ptr<DA::AffectsRel> makeAffects(stmtNo_t stmt1, stmtNo_t stmt2) {
			Argument stmtNo1 = { StmtNo(std::to_string(stmt1)) };
			Argument stmtNo2 = { StmtNo(std::to_string(stmt2)) };
			return make_shared<DA::AffectsRel>(stmtNo1, stmtNo2);
		}
public:

	TEST_METHOD(Test_Modified) {
		auto facade = callModified.getFacade;

		auto affects = makeAffects(1, 3);
		auto e = AffectsEvaluator(affects->arg1, affects->arg2, facade);
		bool res = e.evaluateClauseWithNoSyn();
		Assert::IsFalse(res);
	}

	TEST_METHOD(Test_NotModified) {
		auto facade = callNotModified.getFacade;

		auto affects = makeAffects(1, 3);
		auto e = AffectsEvaluator(affects->arg1, affects->arg2, facade);
		bool res = e.evaluateClauseWithNoSyn();
		Assert::IsTrue(res);
	}
	
	
	TEST_METHOD(direct) {
		Context context = Context::direct();
		auto facade = context.getFacade;
		auto cfg = context.getCfgByStmt(1);

		Assert::IsTrue(AffectsStmtStmt(facade, cfg, 1, 2).evaluate());
		Assert::IsFalse(AffectsStmtStmt(facade, cfg, 1, 1).evaluate());
		Assert::IsFalse( AffectsStmtStmt(facade, cfg, 2, 2).evaluate());

		Assert::IsTrue(RelationshipResults{ {"1","2"} } == AffectsStmtSyn(facade, cfg, 1).evaluate());
		Assert::IsTrue(AffectsStmtWild(facade, cfg, 1).evaluate());
		Assert::IsFalse(AffectsStmtWild(facade, cfg,2).evaluate());

		Assert::IsTrue(RelationshipResults{} == AffectsSynStmt(facade, cfg, 1).evaluate());
		Assert::IsTrue(RelationshipResults{ {"1","2"}} == AffectsSynStmt(facade, cfg, 2).evaluate());
		Assert::IsTrue(RelationshipResults{ {"1","2"}} == AffectsSynSyn(facade, cfg).evaluate());
		
		Assert::IsFalse(AffectsWildStmt(facade, cfg, 1).evaluate());
		Assert::IsTrue(AffectsWildStmt(facade,cfg,2).evaluate());
		Assert::IsTrue(AffectsWildWild(facade,cfg).evaluate());

	}

	TEST_METHOD(self) {
		Context context = Context::self();
		auto facade = context.getFacade;
		auto cfg = context.getCfgByStmt(1);
		Assert::IsTrue(AffectsStmtStmt(facade, cfg, 2, 2).evaluate());
		Assert::IsTrue(AffectsWildWild(facade, cfg).evaluate());
	}
	TEST_METHOD(three_rounds) {
		Context context = Context::three_rounds();
		auto facade = context.getFacade;
		auto cfg = context.getCfgByStmt(1);
		Assert::IsTrue(AffectsStmtStmt(facade, cfg, 4, 3).evaluate());
		Assert::IsTrue(AffectsStmtStmt(facade, cfg, 3, 2).evaluate());
	}
	TEST_METHOD(blocked_read) {
		Context context = Context::blocked_read();
		auto facade = context.getFacade;
		auto cfg = context.getCfgByStmt(1);
		Assert::IsFalse(AffectsStmtStmt(facade, cfg, 1, 3).evaluate());
		Assert::IsTrue(RelationshipResults{} == AffectsStmtSyn(facade, cfg, 1).evaluate());
		Assert::IsFalse(AffectsStmtWild(facade, cfg, 1).evaluate());

		Assert::IsTrue(RelationshipResults{} == AffectsSynStmt(facade, cfg, 3).evaluate());
		Assert::IsTrue(RelationshipResults{ {"3","4"} } == AffectsSynSyn(facade, cfg).evaluate());

		Assert::IsFalse(AffectsWildStmt(facade, cfg, 3).evaluate());
		Assert::IsTrue(AffectsWildWild(facade, cfg).evaluate());
	}
	TEST_METHOD(unblocked_print) {
		Context context = Context::unblocked_print();
		auto facade = context.getFacade;
		auto cfg = context.getCfgByStmt(1);
		Assert::IsTrue(AffectsStmtStmt(facade, cfg, 1, 3).evaluate());
		Assert::IsTrue(RelationshipResults{ {"1","3"}} == AffectsStmtSyn(facade, cfg, 1).evaluate());
		Assert::IsTrue(AffectsStmtWild(facade, cfg, 1).evaluate());

		Assert::IsTrue(RelationshipResults{ {"1","3"} } == AffectsSynStmt(facade, cfg, 3).evaluate());
		Assert::IsTrue(RelationshipResults{ {"1","3"},{"3","4"}} == AffectsSynSyn(facade, cfg).evaluate());

		Assert::IsTrue(AffectsWildStmt(facade, cfg, 3).evaluate());
		Assert::IsTrue(AffectsWildWild(facade, cfg).evaluate());

	}
	TEST_METHOD(blocked_call) {
		Context context = Context::blocked_call();
		auto facade = context.getFacade;
		auto cfg = context.getCfgByStmt(1);
		Assert::IsFalse(AffectsStmtStmt(facade, cfg, 1, 3).evaluate());
		Assert::IsTrue(RelationshipResults{} == AffectsStmtSyn(facade, cfg, 1).evaluate());
		Assert::IsFalse(AffectsStmtWild(facade, cfg, 1).evaluate());

		Assert::IsTrue(RelationshipResults{} == AffectsSynStmt(facade, cfg, 3).evaluate());
		Assert::IsTrue(RelationshipResults{ {"3","4"} } == AffectsSynSyn(facade, cfg).evaluate());

		Assert::IsFalse(AffectsWildStmt(facade, cfg, 3).evaluate());
		Assert::IsTrue(AffectsWildWild(facade, cfg).evaluate());
	}
	TEST_METHOD(unblocked_call) {
		Context context = Context::unblocked_call();
		auto facade = context.getFacade;
		auto cfg = context.getCfgByStmt(1);
		Assert::IsTrue(AffectsStmtStmt(facade, cfg, 1, 3).evaluate());
		Assert::IsTrue(RelationshipResults{ {"1","3"} } == AffectsStmtSyn(facade, cfg, 1).evaluate());
		Assert::IsTrue(AffectsStmtWild(facade, cfg, 1).evaluate());

		Assert::IsTrue(RelationshipResults{ {"1","3"} } == AffectsSynStmt(facade, cfg, 3).evaluate());
		Assert::IsTrue(RelationshipResults{ {"1","3"},{"3","4"} } == AffectsSynSyn(facade, cfg).evaluate());

		Assert::IsTrue(AffectsWildStmt(facade, cfg, 3).evaluate());
		Assert::IsTrue(AffectsWildWild(facade, cfg).evaluate());
	}
	TEST_METHOD(spiral_while) {
		Context context = Context::spiral_while();
		auto facade = context.getFacade;
		auto cfg = context.getCfgByStmt(1);
		
		Assert::IsTrue(RelationshipResults{
			{"6","5"},{"5","3"},
			{"3","7"},{"7","2"},
			{"2","8"},
			} == AffectsSynSyn(facade, cfg).evaluate());
		Assert::IsTrue(AffectsWildWild(facade, cfg).evaluate());

	}
	
	
	
	
	};
}