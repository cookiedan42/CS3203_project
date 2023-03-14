#pragma once
#include <iostream>
#include <exception>

using namespace std;

class QuerySemanticException : public exception {
private:
	string errorMessage;

public:
	QuerySemanticException(string message);

	string what();
};