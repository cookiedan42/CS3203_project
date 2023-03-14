#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class StmtNo {
public:
    std::string value;

    bool operator==(const StmtNo& stmtno) const;
    bool operator!=(const StmtNo& stmtno) const;

    StmtNo() = default;
    StmtNo(std::string value);
};
