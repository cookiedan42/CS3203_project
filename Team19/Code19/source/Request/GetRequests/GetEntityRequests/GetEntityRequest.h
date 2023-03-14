#pragma once

#include "../../Request.h"
#include "../../../TypeAlias/TypeAlias.h"
#include "../../../Definitions/Definitions.h"
#include "../../../Entity/Entity.h"
#include "../../../PKB/Storage/EntityStorage.h"

enum EntityMode {
    VARIABLE,
    CONSTANT,
    PROCEDURE,
    WHILE,
    IF,
    CALL,
    STATEMENT,
    READ,
    PRINT,
    ASSIGNMENT,
    CALLBYSTMT,
    CALLSPROC,
    PRINTVAR,
    READVAR
};


class GetEntityRequest : public Request {
public:
    GetEntityRequest(EntityMode entitymode, entityRef VarExtra = "");
    GetEntityRequest();
    EntityMode getEntityMode() const;
    entityRef getVarExtra() const;

    virtual EntityResults execute(EntityStorage& entityStorage) const = 0;
    bool operator!=(const GetEntityRequest& other) const;

    ~GetEntityRequest() = default;

private:
    EntityMode entitymode;
    entityRef varExtra;
};