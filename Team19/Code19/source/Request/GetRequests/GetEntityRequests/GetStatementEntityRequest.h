#pragma once

#include "./GetEntityRequest.h"

class GetStatementEntityRequest : public GetEntityRequest {
public:
    GetStatementEntityRequest(EntityMode type, stmtNoRef VarExtra = "");
    GetStatementEntityRequest();
    EntityResults execute(EntityStorage& entityStorage) const override;
};
