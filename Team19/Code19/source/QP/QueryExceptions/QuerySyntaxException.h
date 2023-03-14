#pragma once
#include <iostream>
#include <exception>

using namespace std;

class QuerySyntaxException : public exception {
private:
	string errorMessage;

public:
	QuerySyntaxException(string message);

	string what();
};