#pragma once

#include "./GetEntityRequest.h"

class GetPrintStatementEntityRequest : public GetEntityRequest {
public:
    GetPrintStatementEntityRequest(EntityMode type);
    EntityResults execute(EntityStorage& entityStorage) const override;
};