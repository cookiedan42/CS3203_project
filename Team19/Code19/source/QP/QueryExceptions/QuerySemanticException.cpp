#include "QuerySemanticException.h"

QuerySemanticException::QuerySemanticException(string message) : errorMessage(message) {}

string QuerySemanticException::what() {
	return errorMessage;
}
