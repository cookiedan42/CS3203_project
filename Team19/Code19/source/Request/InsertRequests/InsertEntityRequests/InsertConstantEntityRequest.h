#pragma once

#include "InsertEntityRequest.h"

class InsertConstantEntityRequest : public InsertEntityRequest {
    Ent::Constant constantEntity;
public:
    InsertConstantEntityRequest(Ent::Constant);

    Ent::Constant getConstant() const;

    void execute(EntityStorage& entityStorage) const override;
};
