#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class SubExp {
public:
    std::string value;

    bool operator==(const SubExp& subExp) const;
    bool operator!=(const SubExp& subExp) const;

    SubExp() = default;
    SubExp(std::string value);
};
