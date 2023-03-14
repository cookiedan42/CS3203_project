#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include "Declaration.h"

using namespace std;

enum class SelectType {
    SINGLE,
    TUPLE,
    BOOLEAN
};

class SelectObject {
public:
 typedef std::pair<Declaration, ArgType> selectPair;
 SelectType type;
 std::vector<selectPair> selectPairs;
 
 SelectObject();
 SelectObject(std::vector<selectPair> selectPairs);
 
 bool operator==(const SelectObject& so) const;
};
