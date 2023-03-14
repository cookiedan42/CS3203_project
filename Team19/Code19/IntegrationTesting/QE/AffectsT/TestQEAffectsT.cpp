#include "stdafx.h"
#include "CppUnitTest.h"
#include <unordered_set>

#include "../AffectsContext.h"
#include "../source/QP/QueryEvaluator/AffectsTEvaluator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace Test_QE_AffectsT {
	TEST_CLASS(Test_QEAffectsT) {

public:

	shared_ptr<DA::AffectsRel> makeAffects(stmtNo_t stmt1, stmtNo_t stmt2) {
		Argument stmtNo1 = { StmtNo(std::to_string(stmt1)) };
		Argument stmtNo2 = { StmtNo(std::to_string(stmt2)) };
		return make_shared<DA::AffectsRel>(stmtNo1, stmtNo2);
	}
	
	TEST_METHOD(Test_direct) {
		Context context = Context::direct();
		auto getFacade = context.getFacade;
		bool res;
		
		auto affects = makeAffects(1,2);
		res = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithNoSyn();
		Assert::IsTrue(res);
		affects = makeAffects(2, 1);
		res = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithNoSyn();
		Assert::IsFalse(res);
		affects = makeAffects(1, 1);
		res = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithNoSyn();
		Assert::IsFalse(res);
		affects = makeAffects(2, 2);
		res = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithNoSyn();
		Assert::IsFalse(res);
	}

	TEST_METHOD(Test_carried) {
		Context context = Context::carried();
		auto getFacade = context.getFacade;

		auto affects = makeAffects(1, 3);
		bool res = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithNoSyn();
	}


	TEST_METHOD(Test_self) {
		Context context = Context::self();
		auto getFacade = context.getFacade;

		auto affects = makeAffects(2, 2);
		bool res = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithNoSyn();
		Assert::IsTrue(res);
	}

	TEST_METHOD(Test_three_rounds) {
		Context context = Context::three_rounds();
		auto getFacade = context.getFacade;

		auto affects = makeAffects(4, 2);
		bool res = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithNoSyn();
	}

	TEST_METHOD(Test_blocked_read) {
		Context context = Context::blocked_read();
		auto getFacade = context.getFacade;
		bool res;

		auto affects = makeAffects(1, 3);
		res = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithNoSyn();
		Assert::IsFalse(res);

		affects = makeAffects(1, 4);
		res = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithNoSyn();
		Assert::IsFalse(res);
	}

	TEST_METHOD(Test_unblocked_print) {
		Context context = Context::unblocked_print();
		auto getFacade = context.getFacade;
		bool res;

		auto affects = makeAffects(1, 3);
		res = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithNoSyn();
		Assert::IsTrue(res);

		affects = makeAffects(1, 4);
		res = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithNoSyn();
		Assert::IsTrue(res);
	}

	TEST_METHOD(Test_blocked_call) {
		Context context = Context::blocked_call();
		auto getFacade = context.getFacade;
		bool res;

		auto affects = makeAffects(1, 3);
		res = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithNoSyn();
		Assert::IsFalse(res);
		affects = makeAffects(1, 4);
		res = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithNoSyn();
		Assert::IsFalse(res);
		affects = makeAffects(1, 5);
		res = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithNoSyn();
		Assert::IsFalse(res);
	}

	TEST_METHOD(Test_unblocked_call) {
		Context context = Context::unblocked_call();
		auto getFacade = context.getFacade;
		bool res;

		auto affects = makeAffects(1, 3);
		res = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithNoSyn();
		Assert::IsTrue(res);
		affects = makeAffects(1, 4);
		res = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithNoSyn();
		Assert::IsTrue(res);
		affects = makeAffects(1, 5);
		res = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithNoSyn();
		Assert::IsFalse(res);
	}

	TEST_METHOD(Test_spiral_while) {
		Context context = Context::spiral_while();
		auto getFacade = context.getFacade;
		bool res;

		auto affects = makeAffects(6, 5);
		res = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithNoSyn();
		Assert::IsTrue(res);
		affects = makeAffects(6, 3);
		res = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithNoSyn();
		Assert::IsTrue(res);
		affects = makeAffects(6, 7);
		res = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithNoSyn();
		Assert::IsTrue(res);
		affects = makeAffects(6, 2);
		res = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithNoSyn();
		Assert::IsTrue(res);
		affects = makeAffects(6, 8);
		res = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithNoSyn();
		Assert::IsTrue(res);

		affects = makeAffects(5, 3);
		res = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithNoSyn();
		Assert::IsTrue(res);
		affects = makeAffects(5, 7);
		res = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithNoSyn();
		Assert::IsTrue(res);
		affects = makeAffects(5, 2);
		res = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithNoSyn();
		Assert::IsTrue(res);
		affects = makeAffects(5, 8);
		res = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithNoSyn();
		Assert::IsTrue(res);

		affects = makeAffects(3, 7);
		res = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithNoSyn();
		Assert::IsTrue(res);
		affects = makeAffects(3, 2);
		res = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithNoSyn();
		Assert::IsTrue(res);
		affects = makeAffects(3, 8);
		res = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithNoSyn();
		Assert::IsTrue(res);

		affects = makeAffects(7, 2);
		res = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithNoSyn();
		Assert::IsTrue(res);
		affects = makeAffects(7, 8);
		res = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithNoSyn();
		Assert::IsTrue(res);
		affects = makeAffects(2, 8);
		res = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithNoSyn();
		Assert::IsTrue(res);
	
	}
	TEST_METHOD(Test_stmt_stmt_whileBackNest) {
		auto source = 
			"procedure proc {"
			"while (a == a){ "	//1
			"	c=b;"			//2	
			"	while(a==a){"	//3
			"		c = b; "	//4
			"		b = a;"		//5
			"		a = 1;"		//6
			"	} "
			"}"
			"read b; "			//7
			"c=b;"				//8
			"}";
		Context context = Context(source);
		auto getFacade = context.getFacade;
		auto& cfg = context.getCfgByStmt(1);
		

		{
			auto affects = makeAffects(6, 2);
			bool res = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithNoSyn();
			Assert::IsTrue(res);
		}
		
		{
			auto affects = makeAffects(6, 4);
			bool res = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithNoSyn();
			Assert::IsTrue(res);
		}
		{
			auto affects = makeAffects(6, 8);
			bool res = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithNoSyn();
			Assert::IsFalse(res);
		}
		{
			auto affects = makeAffects(5, 8);
			bool res = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithNoSyn();
			Assert::IsFalse(res);
		}
	}


};
};