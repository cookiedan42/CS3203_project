#include "StmtNo.h"


StmtNo::StmtNo(std::string value)
{
	this->value = value;
}

bool StmtNo::operator==(const StmtNo& stmtno) const {
	return tie(value) == tie(stmtno.value);
}

bool StmtNo::operator!=(const StmtNo& stmtno) const {
	return (tie(value) != tie(stmtno.value));
}
