#pragma once

#include "InsertEntityRequest.h"

class InsertVariableEntityRequest : public InsertEntityRequest {
    Ent::Variable variableEntity;
public:
    InsertVariableEntityRequest(Ent::Variable);

    Ent::Variable getVariable() const;

    void execute(EntityStorage& entityStorage) const override;
};