#pragma once

#include "./GetEntityRequest.h"

class GetReadStatementEntityRequest : public GetEntityRequest {
public:
    GetReadStatementEntityRequest(EntityMode type);
    EntityResults execute(EntityStorage& entityStorage) const override;
};