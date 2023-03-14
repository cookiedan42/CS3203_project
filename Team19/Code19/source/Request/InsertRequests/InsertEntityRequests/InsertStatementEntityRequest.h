#pragma once

#include "InsertEntityRequest.h"

class InsertStatementEntityRequest : public InsertEntityRequest {
    Ent::Statement statementEntity;
public:
    InsertStatementEntityRequest(Ent::Statement);

    Ent::Statement getStatement() const;

    void execute(EntityStorage& entityStorage) const override;
};