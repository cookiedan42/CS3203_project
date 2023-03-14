#include "ParseException.h"

ParseException::ParseException(string message) : errorMessage(message) {}

string ParseException::what() {
	return errorMessage;
}
