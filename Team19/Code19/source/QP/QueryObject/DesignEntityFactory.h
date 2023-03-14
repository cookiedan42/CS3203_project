#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <memory>
#include <unordered_set>
#include "DesignEntity.h"
#include "../QueryExceptions/QuerySyntaxException.h"

using namespace std;


class DesignEntityFactory {
public:
	static shared_ptr<DesignEntity> createDesignEntity(std::string synTypeStr);
};