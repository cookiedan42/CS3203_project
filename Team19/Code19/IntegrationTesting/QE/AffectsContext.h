#pragma once
#include "../source/PKB/PKB.h"
#include "../source/PKB/InsertFacade.h"
#include "../source/PKB/GetFacade.h"
#include "../source/SourceProcessor/SPManager.h"


struct Context {
	PKB pkb = PKB();
	InsertFacade insertFacade = InsertFacade(pkb);
	GetFacade getFacade = GetFacade(pkb);

	Context(std::string source) {
		auto spm = SPManager::construct(source);
		spm.populatePKB(insertFacade);
	}

	Cfg& getCfgByStmt(stmtNo_t) {
		auto cfg_request = make_shared<GetCFGRequest>(1);
		auto cfg_result = getFacade.get(cfg_request);
		return *((cfg_result.cbegin())->get());
	}

	static Context direct() {
		auto source = "procedure proc {a=1;b=a;}";
		return Context(source);
	}

	static Context carried() {
		auto source = "procedure proc {a=1;b=a;c=b;}";
		return Context(source);
	}

	static Context self() {
		auto source = "procedure proc {while(a==a){a=a;}}";
		return Context(source);
	}

	static Context three_rounds() {
		auto source = "procedure proc {while(a==a){c=b;b=a;a=1;}}";
		return Context(source);
	}

	static Context blocked_read() {
		auto source = "procedure proc {a=1;read a; b=a;c=b;}";
		return Context(source);
	}

	static Context unblocked_print() {
		auto source = "procedure proc {a=1;print a; b=a;c=b;}";
		return Context(source);
	}

	static Context blocked_call() {
		auto source =
			"procedure proc1 {a=1;call proc2; b=a;c=b;}"
			"procedure proc2 {a=1;}";
		return Context(source);
	}

	static Context unblocked_call() {
		auto source =
			"procedure proc1 {a=1;call proc2; b=a;c=b;}"
			"procedure proc2 {b=1;}";
		return Context(source);
	}

	// 6-->5-->3-->7-->2-->8
	static Context spiral_while() {
		auto source =
			"procedure proc {"
			"while (a==a){"	//1
			"	e=d;"		//2
			"	c=b;"		//3
			"	while (a==a){"//4
			"	b=a;"		//5
			"	a=1;"		//6
			"	}"
			"	d=c;"		//7
			"}"
			"f=e;"			//8
			" }";
		return Context(source);
	}

};