#pragma once

#include "./GetEntityRequest.h"

class GetWhileStatementEntityRequest : public GetEntityRequest {
public:
    GetWhileStatementEntityRequest();
    EntityResults execute(EntityStorage& entityStorage) const override;
};
