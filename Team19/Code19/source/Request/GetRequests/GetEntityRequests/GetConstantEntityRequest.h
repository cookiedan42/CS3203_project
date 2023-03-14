#pragma once

#include "./GetEntityRequest.h"

class GetConstantEntityRequest : public GetEntityRequest {
public:
    GetConstantEntityRequest();
    EntityResults execute(EntityStorage& entityStorage) const override;
};
