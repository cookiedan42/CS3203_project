#include "QuerySyntaxException.h"

QuerySyntaxException::QuerySyntaxException(string message) : errorMessage(message) {}

string QuerySyntaxException::what() {
	return errorMessage;
}
