#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class StringExp {
public:
    std::string value;

    bool operator==(const StringExp& stringExp) const;
    bool operator!=(const StringExp& stringExp) const;

    StringExp() = default;
    StringExp(std::string value);
};
