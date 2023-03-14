#include "ValidatorUtil.h"

bool ValidatorUtil::isInteger(const std::string& arg) {
	return std::regex_match(arg, intRegex);
}

bool ValidatorUtil::isName(const std::string& arg) {
	return std::regex_match(arg, nameRegex);
}

bool ValidatorUtil::isBoolean(const std::string& arg) {
	return std::regex_match(arg, booleanRegex);
}

bool ValidatorUtil::isComparator(const std::string& arg) {
	return std::regex_match(arg, comparatorRegex);
}

bool ValidatorUtil::isMathOperator(const std::string& arg) {
	return std::regex_match(arg, mathOperatorRegex);
}

bool ValidatorUtil::verifyName(std::string s) {
	return isName(s);
}

bool ValidatorUtil::verifyInt(std::string s) {
	return isInteger(s);
}
