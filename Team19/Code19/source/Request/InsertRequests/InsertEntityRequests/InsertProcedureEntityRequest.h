#pragma once

#include "InsertEntityRequest.h"


class InsertProcedureEntityRequest : public InsertEntityRequest {
    Ent::Procedure  procedureEntity;
public:
    InsertProcedureEntityRequest(Ent::Procedure);

    Ent::Procedure getProcedure() const;

    void execute(EntityStorage& entityStorage) const override;
};
