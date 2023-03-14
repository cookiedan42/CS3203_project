#include "GetCFGRequest.h"

GetCFGRequest::GetCFGRequest() :
	mode(CFGMode::ALL), procName(EMPTY_PROCNAME), stmtNo(EMPTY_STMTNO) {}

GetCFGRequest::GetCFGRequest(procName_t procName) :
	mode(CFGMode::FILTER_PROC), procName(procName), stmtNo(EMPTY_STMTNO) {}

GetCFGRequest::GetCFGRequest(stmtNo_t stmtNo) :
	mode(CFGMode::FILTER_STMT_NO), procName(EMPTY_PROCNAME), stmtNo(stmtNo) {}

CFGMode GetCFGRequest::getMode() const {
	return this->mode;
}

procName_t GetCFGRequest::getProcName() const {
	return this->procName;
}

stmtNo_t GetCFGRequest::getStmtNo() const {
	return this->stmtNo;
}

CFGResults GetCFGRequest::execute(RelationshipStorage& storage) {
	CFGMode mode = this->getMode();

	switch (mode) {
	case CFGMode::FILTER_PROC:
	{
		auto& CFGStorage = storage.getCFGStorage();
		return CFGStorage.getCFGByProc(this->procName);
	}
	case CFGMode::FILTER_STMT_NO:
	{
		auto& CFGStorage = storage.getCFGStorage();
		return CFGStorage.getCFGByStmtNo(this->stmtNo);
	}
	default:
		auto& CFGStorage = storage.getCFGStorage();
		return CFGStorage.getAllCFG();
	}
}