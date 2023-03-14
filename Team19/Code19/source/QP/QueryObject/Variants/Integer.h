#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Integer {
public:
    std::string value;

    bool operator==(const Integer& integer) const;
    bool operator!=(const Integer& integer) const;

    Integer() = default;
    Integer(std::string value);
};
