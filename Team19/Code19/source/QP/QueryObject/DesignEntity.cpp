#include "DesignEntity.h"

using namespace DE;

DesignEntity::DesignEntity(std::string type) : type(type) {}
DE::Procedure::Procedure() : DesignEntity(PROCEDURE_DE) {}
DE::Assign::Assign() : DesignEntity(ASSIGN_DE) {}
DE::Call::Call() : DesignEntity(CALL_DE) {}
DE::Constant::Constant() : DesignEntity(CONSTANT_DE) {}
DE::If::If() : DesignEntity(IF_DE) {}
DE::While::While() : DesignEntity(WHILE_DE) {}
DE::Print::Print() : DesignEntity(PRINT_DE) {}
DE::Read::Read() : DesignEntity(READ_DE) {}
DE::Statement::Statement() : DesignEntity(STATEMENT_DE) {}
DE::Variable::Variable() : DesignEntity(VARIABLE_DE) {}

bool DesignEntity::operator==(const DesignEntity& de) const {
	return tie(type) == tie(de.type);
}

bool DesignEntity::operator!=(const DesignEntity& dec) const {
	return tie(type) != tie(dec.type);
}


DesignEntity::synonym_values DesignEntity::getSynonymFromPKB(GetFacade* facade) {
	return {};
}

DesignEntity::synonym_attrRef DesignEntity::getSynonymWithAttrRefFromPKB(GetFacade* facade, std::string synHeader, std::string attrHeader) {
	return {};
}

// Procedure
DesignEntity::synonym_values DE::Procedure::getSynonymFromPKB(GetFacade* facade) {
	auto request = std::make_shared<GetProcedureEntityRequest>();
	return facade->get(request);
}

DesignEntity::synonym_attrRef DE::Procedure::getSynonymWithAttrRefFromPKB(GetFacade* facade, std::string synHeader, std::string attrHeader) {
	// get all procedure names (procedure.procName)
	vector<string> headers;
	headers.push_back(synHeader);
	headers.push_back(attrHeader);
	
	auto request = std::make_shared<GetProcedureEntityRequest>();
	unordered_set<string> procNames = facade->get(request);
	vector<vector<string>> items;
	for (auto& procName : procNames)
	{
		vector<string> item;
		item.push_back(procName);
		item.push_back(procName);
		items.push_back(item);

	}

	return items;
}

// Statement
DesignEntity::synonym_values DE::Statement::getSynonymFromPKB(GetFacade* facade) {
	auto request = std::make_shared<GetStatementEntityRequest>(EntityMode::STATEMENT);
	return facade->get(request);
}

DesignEntity::synonym_attrRef DE::Statement::getSynonymWithAttrRefFromPKB(GetFacade* facade, std::string synHeader, std::string attrHeader) {
	// get all statement numbers (statement.stmtno)
	vector<string> headers;
	headers.push_back(synHeader);
	headers.push_back(attrHeader);

	auto request = std::make_shared<GetStatementEntityRequest>(EntityMode::STATEMENT);
	unordered_set<string> stmtnos = facade->get(request);
	vector<vector<string>> items;
	for (auto& stmtno : stmtnos)
	{
		vector<string> item;
		item.push_back(stmtno);
		item.push_back(stmtno);
		items.push_back(item);

	}

	return items;
}

// Read
DesignEntity::synonym_values DE::Read::getSynonymFromPKB(GetFacade* facade) {
	auto request = std::make_shared<GetReadStatementEntityRequest>(EntityMode::READ);
	return facade->get(request);
}

DesignEntity::synonym_attrRef DE::Read::getSynonymWithAttrRefFromPKB(GetFacade* facade, std::string synHeader, std::string attrHeader) {
	// get all read statement numbers and variable read (read.varName)
	vector<string> headers;
	headers.push_back(synHeader);
	headers.push_back(attrHeader);

	auto request = std::make_shared<GetReadStatementEntityRequest>(EntityMode::READ);
	unordered_set<string> stmtnos = facade->get(request);
	vector<vector<string>> items;
	for (auto& stmtno : stmtnos)
	{
		// get variable read
		auto request = std::make_shared<GetModifiesRequest>(stmtno, Mode::FILTER_ARG_1, Arg1Type::STATEMENT);
		RelationshipResults result = facade->get(request);
		for (auto& [k, v] : result) 
		{
			vector<string> item;
			item.push_back(stmtno);
			item.push_back(v);
			items.push_back(item);
		}
	}

	return items;
}


// Print
DesignEntity::synonym_values DE::Print::getSynonymFromPKB(GetFacade* facade) {
	auto request = std::make_shared<GetPrintStatementEntityRequest>(EntityMode::PRINT);
	return facade->get(request);
}

DesignEntity::synonym_attrRef DE::Print::getSynonymWithAttrRefFromPKB(GetFacade* facade, std::string synHeader, std::string attrHeader) {
	// get all print statement numbers and variable printed (print.varName)
	vector<string> headers;
	headers.push_back(synHeader);
	headers.push_back(attrHeader);

	auto request = std::make_shared<GetPrintStatementEntityRequest>(EntityMode::PRINT);
	unordered_set<string> stmtnos = facade->get(request);
	vector<vector<string>> items;
	for (auto& stmtno : stmtnos)
	{
		// get variable printed
		auto request = std::make_shared<GetUsesRequest>(stmtno, Mode::FILTER_ARG_1, Arg1Type::STATEMENT);
		RelationshipResults result = facade->get(request);
		for (auto& [k, v] : result)
		{
			vector<string> item;
			item.push_back(stmtno);
			item.push_back(v);
			items.push_back(item);
		}
	}

	return items;
}

// Assign
DesignEntity::synonym_values DE::Assign::getSynonymFromPKB(GetFacade* facade) {
	auto request = std::make_shared<GetAssignStatementEntityRequest>();
	return facade->get(request);
}

DesignEntity::synonym_attrRef DE::Assign::getSynonymWithAttrRefFromPKB(GetFacade* facade, std::string synHeader, std::string attrHeader) {
	// get all assign stmt numbers (assign.stmtno)
	vector<string> headers;
	headers.push_back(synHeader);
	headers.push_back(attrHeader);

	auto request = std::make_shared<GetAssignStatementEntityRequest>();
	unordered_set<string> stmtnos = facade->get(request);
	vector<vector<string>> items;
	for (auto& stmtno : stmtnos)
	{
		vector<string> item;
		item.push_back(stmtno);
		item.push_back(stmtno);
		items.push_back(item);

	}

	return items;
}

// Call
DesignEntity::synonym_values DE::Call::getSynonymFromPKB(GetFacade* facade) {
	auto request = std::make_shared<GetCallStatementEntityRequest>(EntityMode::CALL);
	return facade->get(request);
}

DesignEntity::synonym_attrRef DE::Call::getSynonymWithAttrRefFromPKB(GetFacade* facade, std::string synHeader, std::string attrHeader) {
	// get all call statement numbers and procName called (call.procName)
	vector<string> headers;
	headers.push_back(synHeader);
	headers.push_back(attrHeader);

	auto request = std::make_shared<GetCallStatementEntityRequest>(EntityMode::CALL);
	unordered_set<string> stmtnos = facade->get(request);
	vector<vector<string>> items;
	for (auto& stmtno : stmtnos)
	{
		// get procName called
		auto request = std::make_shared<::GetCallStatementEntityRequest>(EntityMode::CALLBYSTMT, stmtno);
		std::string procName = *facade->get(request).begin();
		vector<string> item;
		item.push_back(stmtno);
		item.push_back(procName);
		items.push_back(item);
	}

	return items;
}

// While
DesignEntity::synonym_values DE::While::getSynonymFromPKB(GetFacade* facade) {
	auto request = std::make_shared<GetWhileStatementEntityRequest>();
	return facade->get(request);
}

DesignEntity::synonym_attrRef DE::While::getSynonymWithAttrRefFromPKB(GetFacade* facade, std::string synHeader, std::string attrHeader) {
	// get all while stmt numbers (while.stmtno)
	vector<string> headers;
	headers.push_back(synHeader);
	headers.push_back(attrHeader);

	auto request = std::make_shared<GetWhileStatementEntityRequest>();
	unordered_set<string> stmtnos = facade->get(request);
	vector<vector<string>> items;
	for (auto& stmtno : stmtnos)
	{
		vector<string> item;
		item.push_back(stmtno);
		item.push_back(stmtno);
		items.push_back(item);

	}

	return items;
}

// If
DesignEntity::synonym_values DE::If::getSynonymFromPKB(GetFacade* facade) {
	auto request = std::make_shared<GetIfStatementEntityRequest>();
	return facade->get(request);
}

DesignEntity::synonym_attrRef DE::If::getSynonymWithAttrRefFromPKB(GetFacade* facade, std::string synHeader, std::string attrHeader) {
	// get all if stmt numbers (if.stmtno)
	vector<string> headers;
	headers.push_back(synHeader);
	headers.push_back(attrHeader);

	auto request = std::make_shared<GetIfStatementEntityRequest>();
	unordered_set<string> stmtnos = facade->get(request);
	vector<vector<string>> items;
	for (auto& stmtno : stmtnos)
	{
		vector<string> item;
		item.push_back(stmtno);
		item.push_back(stmtno);
		items.push_back(item);

	}

	return items;
}

// Variable
DesignEntity::synonym_values DE::Variable::getSynonymFromPKB(GetFacade* facade) {
	auto request = std::make_shared<GetVariableEntityRequest>();
	return facade->get(request);
}

DesignEntity::synonym_attrRef DE::Variable::getSynonymWithAttrRefFromPKB(GetFacade* facade, std::string synHeader, std::string attrHeader) {
	// get all variable names (variable.varName)
	vector<string> headers;
	headers.push_back(synHeader);
	headers.push_back(attrHeader);

	auto request = std::make_shared<GetVariableEntityRequest>();
	unordered_set<string> varNames = facade->get(request);
	vector<vector<string>> items;
	for (auto& varName : varNames)
	{
		vector<string> item;
		item.push_back(varName);
		item.push_back(varName);
		items.push_back(item);

	}

	return items;
}

// Constant 
DesignEntity::synonym_values DE::Constant::getSynonymFromPKB(GetFacade* facade) {
	auto request = std::make_shared<GetConstantEntityRequest>();
	return facade->get(request);
}

DesignEntity::synonym_attrRef DE::Constant::getSynonymWithAttrRefFromPKB(GetFacade* facade, std::string synHeader, std::string attrHeader) {
	// get all variable names (variable.varName)
	vector<string> headers;
	headers.push_back(synHeader);
	headers.push_back(attrHeader);

	auto request = std::make_shared<GetConstantEntityRequest>();
	unordered_set<string> consts = facade->get(request);
	vector<vector<string>> items;
	for (auto& c : consts)
	{
		vector<string> item;
		item.push_back(c);
		item.push_back(c);
		items.push_back(item);

	}

	return items;
}
