#include "stdafx.h"
#include "CppUnitTest.h"
#include <stdexcept>
#include "../source/SourceProcessor/Validator/Validator.h"
#include "../source/SourceProcessor/Tokenizer/SPTokenizer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(Test_SP_Validator) {
public:

	TEST_METHOD(Test_Empty_Program_fail) {
		Assert::ExpectException<validation_error>([]() {
			auto t = TokenSplitter("").tokenize();
			Validator(t).validateProgram();
		});
	}

	TEST_METHOD(Test_Procedure_empty_fail) {
		Assert::ExpectException<validation_error>([]() {
			auto t = TokenSplitter("procedure name{}").tokenize();
			Validator(t).validateProgram();
		});
		auto t = TokenSplitter("procedure name{read a;}").tokenize();
		Validator(t).validateProgram();

	}

	TEST_METHOD(Test_Procedure_name) {
		auto t = TokenSplitter("procedure procedure{read a;}").tokenize();
		Validator(t).validateProgram();

		Assert::ExpectException<validation_error>([]() {
			auto t = TokenSplitter("procedure{read a;}").tokenize();
			Validator(t).validateProgram();
		});
		Assert::ExpectException<validation_error>([]() {
			auto t = TokenSplitter("procedure name name {read a;}").tokenize();
			Validator(t).validateProgram();
		});
	}

	TEST_METHOD(Test_SimpleStatements) {
		std::vector<std::string> valid_stmt = {
			"read a;", "read abc;","read a1233;",
			"print a;", "print abc;","print a1233;"
			"read read;","print print;"
		};
		for each (auto stmt in valid_stmt) {
			stmt = "procedure name{" + stmt + "}";
			auto t = TokenSplitter(stmt).tokenize();
			Validator(t).validateProgram();
		}
	}

	TEST_METHOD(Test_Procedure_SimpleStatements) {
		std::vector<std::string> valid_programs = {
		"procedure name{read read; read print;}",
		"procedure name{print print; print read;}"
		};
		for each (auto program in valid_programs) {
			auto t = TokenSplitter(program).tokenize();
			Validator(t).validateProgram();
		}

		std::vector<std::string> invalid_programs = {
		"procedure name{read;}",
		"procedure name{print;}",
		"procedure name{print read read;}",
		"procedure name{read print print;}",
		"procedure name{print print print;}",
		"procedure name{read read read;}",
		};
		for each (auto prog in invalid_programs) {
			Assert::ExpectException<validation_error>([prog]() {
				auto t = TokenSplitter(prog).tokenize();
				Validator(t).validateProgram();
			});
		}
	}

	TEST_METHOD(Test_AssignStatements) {
		std::vector<std::string> valid_assigns = {
		"a = 5;","a = b;",
		"ab = 12;", "a12 = a13;",
		"ab = 1+2;","a1 = ((1*2));",
		"a=0;"
		};
		for each (auto assign in valid_assigns) {
			assign = "procedure name{" + assign + "}";
			auto t = TokenSplitter(assign).tokenize();
			Validator(t).validateProgram();
		}
		std::vector<std::string> invalid_assigns = {
		"a==b;","a==1;","a a = RHS;", "a=R H;",
		"1a=d;","a=1a;", "a=01;"
		};
		for each (auto assign in invalid_assigns) {
			Assert::ExpectException<validation_error>([assign]() {
				auto assign1 = "procedure name{" + assign + "}";
				auto t = TokenSplitter(assign1).tokenize();
				Validator(t).validateStatement();
			});
		}
	}

	TEST_METHOD(Test_Procedure_NestedStatements) {
		std::vector<std::string> valid_programs = {
			"procedure name{if(a>b)then{read a;}else{read a;}}",
			"procedure name{if(a>b)then{read a;read b;}else{read a; print b;}}",
			"procedure name{while(a>b){read a;}}",
			"procedure name{while(a>b){read a;read b;}}"
		};
		for each (auto program in valid_programs) {
			auto t = TokenSplitter(program).tokenize();
			Validator(t).validateProgram();
		}
	}

	TEST_METHOD(Test_NestedStatements) {
		std::vector<std::string> valid_nests = {
				"if(a>b)then{read a;}else{read a;}",
				"if(a>b)then{read a;read b;}else{read a; print b;}",
				"while(a>b){read a;}",
				"while (a>b) { read a; read b; }"
		};
		for each (auto program in valid_nests) {
			program = "procedure name{" + program + "}";
			auto t = TokenSplitter(program).tokenize();
			Validator(t).validateProgram();
		}

		std::vector<std::string> invalid_nests = {
			"if(a>b)then{}else{}",
			"if(a>b)then{read a;}else{}",
			"if(a>b)then{}else{read a;}",
			"while(a>b){}"
		};
		for each (auto nest in invalid_nests) {
			Assert::ExpectException<validation_error>([nest]() {
				auto nest1 = "procedure name{" + nest + "}";
				auto t = TokenSplitter(nest1).tokenize();
				Validator(t).validateProgram();
				});
		}
	}


	TEST_METHOD(Test_Illegal_Char) {
		Assert::ExpectException<validation_error>([]() {
			auto t = TokenSplitter("procedure procedure{read _;}").tokenize();
			Validator(t).validateProgram();
		});
	}


	TEST_METHOD(Test_Validate_nesting) {
		auto t = TokenSplitter("procedure procedure{read a;}").tokenize();
		Validator(t).validateProgram();


		Assert::ExpectException<validation_error>([]() {
			auto t = TokenSplitter("procedure procedure{procedure procedure{read a;}}").tokenize();
			Validator(t).validateProgram();
		});
	}

	TEST_METHOD(Test_Validate_recursive_throwError) {
		Assert::ExpectException<validation_error>([]() {
			auto t = TokenSplitter("procedure procedure{call procdure;}").tokenize();
			Validator(t).validateProgram();
			});
	}

	TEST_METHOD(Test_Validate_loop_throwError) {
		Assert::ExpectException<validation_error>([]() {
			auto t = TokenSplitter("procedure procedure{call proc2;}procedure proc2{call procedure;}").tokenize();
			Validator(t).validateProgram();
		});
	}

	TEST_METHOD(Test_Validate_invalidCall_throwError) {
		Assert::ExpectException<validation_error>([]() {
			auto t = TokenSplitter("procedure procedure{call proc2;}").tokenize();
			Validator(t).validateProgram();
		});

		Assert::ExpectException<validation_error>([]() {
			auto t = TokenSplitter("procedure procedure{call proc2;}procedure proc2{call proc4;}").tokenize();
			Validator(t).validateProgram();
		});
	}


};
}