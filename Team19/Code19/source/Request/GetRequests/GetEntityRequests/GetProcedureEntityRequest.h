#pragma once

#include "./GetEntityRequest.h"

class GetProcedureEntityRequest : public GetEntityRequest {
public:
    GetProcedureEntityRequest();
    EntityResults execute(EntityStorage& entityStorage) const override;
};
