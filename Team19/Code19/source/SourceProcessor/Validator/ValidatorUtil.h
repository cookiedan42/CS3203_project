#pragma once
#include <regex>
#include <string>

class ValidatorUtil {
	inline static std::regex intRegex = std::regex("0|[1-9][0-9]*");
	inline static std::regex nameRegex = std::regex("[a-zA-Z][a-zA-Z0-9]*");
	inline static std::regex booleanRegex = std::regex("\\|\\||&&");
	inline static std::regex comparatorRegex = std::regex("((>|<)=?)|((=|!)=)");
	inline static std::regex mathOperatorRegex = std::regex("\\+|-|\\*|\\/|%");

public:
	// 0 or 
	// nonzero followed by digits
	static bool isInteger(const std::string& arg);

	// start with letter
	// followed by any number of alphanumeric
	static bool isName(const std::string& arg);

	static bool isBoolean(const std::string& arg);
	
	static bool isComparator(const std::string& arg);

	static bool isMathOperator(const std::string& arg);

	static bool verifyName(std::string s);

	static bool verifyInt(std::string s);
};

