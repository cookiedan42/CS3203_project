#pragma once

#include "./GetEntityRequest.h"

class GetAssignStatementEntityRequest : public GetEntityRequest {
public:
    GetAssignStatementEntityRequest();
    EntityResults execute(EntityStorage& entityStorage) const override;
};