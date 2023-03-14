#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "DesignEntity.h"

using namespace std;

class Declaration {
    public:

        shared_ptr<DesignEntity> synType;
        std::string synName;

        bool operator==(const Declaration& dec) const;
        bool operator!=(const Declaration& dec) const;

        Declaration();
        Declaration(shared_ptr<DesignEntity> type, std::string name);
};
