#pragma once

#include "./GetEntityRequest.h"

class GetCallStatementEntityRequest : public GetEntityRequest {
public:
    GetCallStatementEntityRequest(EntityMode type, entityRef VarExtra = "");
    EntityResults execute(EntityStorage& entityStorage) const override;
};
