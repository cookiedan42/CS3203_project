#pragma once

#include "../../../Definitions/Definitions.h"
#include "../../../TypeAlias/TypeAlias.h"
#include "../../../PKB/Storage/RelationshipStorage.h"
#include "../../Request.h"

class InsertRelationshipRequest : public Request {
public:
    InsertRelationshipRequest() {};

    virtual void execute(RelationshipStorage&) = 0;

    ~InsertRelationshipRequest() = default;

};