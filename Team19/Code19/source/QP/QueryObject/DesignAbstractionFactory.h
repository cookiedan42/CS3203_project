#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <memory>
#include <unordered_set>
#include "DesignAbstraction.h"
#include "../QueryExceptions/QuerySyntaxException.h"

using namespace std;


class DesignAbstractionFactory {
public:
	static shared_ptr<DesignAbstraction> createDesignAbstraction(std::string relationStr, Argument arg1, Argument arg2);
};