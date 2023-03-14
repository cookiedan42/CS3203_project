#include "stdafx.h"
#include "CppUnitTest.h"
#include <unordered_set>

#include "../AffectsContext.h"
#include "../source/QP/QueryEvaluator/AffectsTEvaluator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace Test_QE_AffectsT {
	TEST_CLASS(Test_QEAffectsT_syn_syn) {
		shared_ptr<DA::AffectsRel> makeAffects() {
			Argument s1 = { Declaration(make_shared<DE::Statement>(), "syn1") };
			Argument s2 = { Declaration(make_shared<DE::Statement>(), "syn2") };
			return make_shared<DA::AffectsRel>(s1, s2);
		}
		Table makeTable(RelationshipResults result) {
			return QueryUtil::createTableArg1Arg2("syn1", "syn2", result);
		}
		void assertTableEqual(Table& expected, Table& actual) {
			auto expectedHeader = expected.getTableHeader();
			auto actualHeader = actual.getTableHeader();
			auto expectedRows = expected.getTableRows();
			auto actualRows = actual.getTableRows();

			Assert::IsTrue(expectedHeader == actualHeader);
			Assert::IsTrue(expectedRows.size() == expectedRows.size());
			if (expectedHeader.size() == 0) {
				Assert::IsTrue(expectedRows == actualRows);
			} else if (expectedHeader.size() == 1) {
				unordered_set<string> expectedRowSet;
				unordered_set<string> actualRowSet;
				for (const auto& column : expectedRows) expectedRowSet.insert(*column.cbegin());
				for (const auto& column : actualRows) actualRowSet.insert(*column.cbegin());

				Assert::IsTrue(expectedRowSet == actualRowSet);
			} else {
				unordered_set<pair<string, string>, stringPairHash> expectedRowSet;
				unordered_set<pair<string, string>, stringPairHash> actualRowSet;
				for (const auto& column : expectedRows) expectedRowSet.insert(pair(*column.cbegin(), *column.rbegin()));
				for (const auto& column : actualRows) actualRowSet.insert(pair(*column.cbegin(), *column.rbegin()));

				Assert::IsTrue(expectedRowSet == actualRowSet);
			}
		}


public:
	TEST_METHOD(Test_direct) {
		Context context = Context::direct();
		auto getFacade = context.getFacade;

		auto affects = makeAffects();
		Table actual = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithSyn();
		Table expected =  makeTable({ { "1", "2" } });

		assertTableEqual(expected,actual);
	}

	TEST_METHOD(Test_carried) {
		Context context = Context::carried();
		auto getFacade = context.getFacade;
		
		auto affects = makeAffects();
		Table actual = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithSyn();
		Table expected =  makeTable({ { "1","3"},{ "1","2"},{"2","3"} });
		assertTableEqual(expected, actual);

	}
	TEST_METHOD(Test_self) {
		Context context = Context::self();
		auto getFacade = context.getFacade;
		
		auto affects = makeAffects();
		Table actual = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithSyn();
		Table expected =  makeTable( { { "2","2"},});

		assertTableEqual(expected,actual);

	}
	TEST_METHOD(Test_while) {
		Context context = Context::three_rounds();
		auto getFacade = context.getFacade;

		auto affects = makeAffects();
		Table actual = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithSyn();
		Table expected =  makeTable( { { "4","2"},{ "3","2"},{"4","3"} });
		
		assertTableEqual(expected,actual);
	}


	TEST_METHOD(Test_blocked_read) {
		Context context = Context::blocked_read();
		auto getFacade = context.getFacade;

		auto affects = makeAffects();
		Table actual = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithSyn();
		Table expected =  makeTable( { { "3","4"}, });

		assertTableEqual(expected,actual);
	}

	TEST_METHOD(Test_unblocked_print) {
		Context context = Context::unblocked_print();
		auto getFacade = context.getFacade;

		auto affects = makeAffects();
		Table actual = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithSyn();
		Table expected =  makeTable( { { "1","3"},{ "1","4"},{"3","4"} });

		assertTableEqual(expected,actual);
	}

	TEST_METHOD(Test_blocked_call) {
		Context context = Context::blocked_call();
		auto getFacade = context.getFacade;

		auto affects = makeAffects();
		Table actual = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithSyn();
		Table expected =  makeTable( { { "3","4"}, });

		assertTableEqual(expected,actual);
	}

	TEST_METHOD(Test_unblocked_call) {
		Context context = Context::unblocked_call();
		auto getFacade = context.getFacade;

		auto affects = makeAffects();
		Table actual = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithSyn();
		Table expected =  makeTable( { { "1","3"},{ "1","4"},{"3","4"} });

		assertTableEqual(expected,actual);
	}

	TEST_METHOD(Test_spiral_while) {
		Context context = Context::spiral_while();
		auto getFacade = context.getFacade;

		auto affects = makeAffects();
		Table actual = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithSyn();
		Table expected =  makeTable({
			{ "6","5"},{ "6","3"},{ "6","7"},{ "6","2"},{ "6","8"},
			{ "5","3"},{ "5","7"},{ "5","2"},{ "5","8"},
			{ "3","7"},{ "3","2"},{ "3","8"},
			{ "7","2"},{ "7","8"},
			{ "2","8"},
		});
		assertTableEqual(expected,actual);
	}




};
};