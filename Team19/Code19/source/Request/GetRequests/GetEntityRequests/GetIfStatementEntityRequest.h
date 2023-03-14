#pragma once

#include "./GetEntityRequest.h"

class GetIfStatementEntityRequest : public GetEntityRequest {
public:
    GetIfStatementEntityRequest();
    EntityResults execute(EntityStorage& entityStorage) const override;
};
