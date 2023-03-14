#pragma once

#include "./GetEntityRequest.h"

class GetVariableEntityRequest: public GetEntityRequest{
public:
    GetVariableEntityRequest();
    EntityResults execute(EntityStorage& entityStorage) const override;
};
