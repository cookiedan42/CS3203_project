#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include "QueryUtil.h"
#include "../QueryObject/SelectObject.h"

using namespace std;

class ResultConsolidator {

    public:
        static list<string> selectBoolean(bool evalResult);
        static list<string> selectTuple(SelectObject selectObj, Table table);
 
    private:
        // Disallow creating an instance of this object
        ResultConsolidator() {}
};

