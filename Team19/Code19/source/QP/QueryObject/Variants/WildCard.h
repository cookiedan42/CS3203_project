#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class WildCard {
public:

    WildCard();

    bool operator==(const WildCard& wildcard) const;
};
