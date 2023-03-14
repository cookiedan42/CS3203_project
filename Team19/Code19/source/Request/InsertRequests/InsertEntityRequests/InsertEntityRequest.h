#pragma once

#include "../../Request.h"
#include "../../../TypeAlias/TypeAlias.h"
#include "../../../Definitions/Definitions.h"
#include "../../../Entity/Entity.h"
#include "../../../PKB/Storage/EntityStorage.h"
#include <vector>



class InsertEntityRequest : public Request {
public:
    InsertEntityRequest() {};

    virtual void execute(EntityStorage& entityStorage) const = 0;
    bool operator!=(const InsertEntityRequest& other) const;

    ~InsertEntityRequest() = default;

};