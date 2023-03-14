#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/SourceProcessor/Validator/CallValidator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(Test_SP_CallValidator) {
public:
	TEST_METHOD(Test_all_proc1callproc2_true) {
		auto cv = CallValidator();
		cv.setProc("proc1");
		cv.addCall("proc2");
		cv.setProc("proc2");

		Assert::IsTrue(cv.noInvalid());
		Assert::IsTrue(cv.noRecurse());
		Assert::IsTrue(cv.noLoop());
		Assert::IsTrue(cv.isValid());
	}

	TEST_METHOD(Test_all_fullyConnectedDG_true) {
		auto cv = CallValidator();
		
		cv.setProc("proc2");
		cv.addCall("proc3");

		cv.setProc("proc3");

		cv.setProc("proc1");
		cv.addCall("proc2");
		cv.addCall("proc3");

		Assert::IsTrue(cv.noInvalid());
		Assert::IsTrue(cv.noRecurse());
		Assert::IsTrue(cv.noLoop());
		Assert::IsTrue(cv.isValid());
	}

	TEST_METHOD(Test_recurse_proc1Callproc1_isfalse) {
		auto cv = CallValidator();
		cv.setProc("proc1");
		cv.addCall("proc1");
		cv.setProc("proc2");

		Assert::IsTrue(cv.noInvalid());
		Assert::IsFalse(cv.noRecurse());
		Assert::IsFalse(cv.noLoop()); // recursion is a loop
		Assert::IsFalse(cv.isValid());

	}

	TEST_METHOD(Test_noloop_loop_noloopFalse) {
		auto cv = CallValidator();
		cv.setProc("proc1");
		cv.addCall("proc2");
		cv.setProc("proc2");
		cv.addCall("proc1");

		Assert::IsTrue(cv.noInvalid());
		Assert::IsTrue(cv.noRecurse());
		Assert::IsFalse(cv.noLoop());
		Assert::IsFalse(cv.isValid());

	}

	TEST_METHOD(Test_noInvalid_invalidCall_throwError) {
		auto cv = CallValidator();
		cv.setProc("proc1");
		cv.addCall("proc3");
		cv.setProc("proc2");

		Assert::IsFalse(cv.noInvalid());
		Assert::IsTrue(cv.noRecurse());
		// Assert::IsTrue(cv.no_loop());
		// no_loop should not be called when there is invalid
		Assert::IsFalse(cv.isValid());
	}

	TEST_METHOD(Test_duplicateProc_throwError) {
		Assert::ExpectException<validation_error>([]() {
			auto cv = CallValidator();
			cv.setProc("proc1");
			cv.setProc("proc1");
			});
	}

};
}