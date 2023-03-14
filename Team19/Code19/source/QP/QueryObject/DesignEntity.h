#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <unordered_set>
#include "../../PKB/GetFacade.h"
#include "../../Request/GetRequests/GetEntityRequests/GetProcedureEntityRequest.h"
#include "../../Request/GetRequests/GetEntityRequests/GetStatementEntityRequest.h"
#include "../../Request/GetRequests/GetEntityRequests/GetReadStatementEntityRequest.h"
#include "../../Request/GetRequests/GetEntityRequests/GetPrintStatementEntityRequest.h"
#include "../../Request/GetRequests/GetEntityRequests/GetAssignStatementEntityRequest.h"
#include "../../Request/GetRequests/GetEntityRequests/GetCallStatementEntityRequest.h"
#include "../../Request/GetRequests/GetEntityRequests/GetWhileStatementEntityRequest.h"
#include "../../Request/GetRequests/GetEntityRequests/GetIfStatementEntityRequest.h"
#include "../../Request/GetRequests/GetEntityRequests/GetVariableEntityRequest.h"
#include "../../Request/GetRequests/GetEntityRequests/GetConstantEntityRequest.h"
#include "../../Request/GetRequests/GetRelationshipRequests/GetModifiesRequest.h"
#include "../../Request/GetRequests/GetRelationshipRequests/GetUsesRequest.h"

using namespace std;

class DesignEntity {
public:
	std::string type;
	typedef std::unordered_set<std::string> synonym_values;
	typedef std::vector<std::vector<std::string>> synonym_attrRef;

	DesignEntity() = default;
	DesignEntity(std::string type);

	bool operator==(const DesignEntity& de) const;
	bool operator!=(const DesignEntity& de) const;
	
	virtual synonym_values getSynonymFromPKB(GetFacade* facade);
	virtual synonym_attrRef getSynonymWithAttrRefFromPKB(GetFacade* facade, std::string synHeader, std::string attrHeader);
};


namespace DE {
	class Procedure : public DesignEntity {
	public:
		Procedure();
		synonym_values getSynonymFromPKB(GetFacade* facade) override;
		synonym_attrRef getSynonymWithAttrRefFromPKB(GetFacade* facade, std::string synHeader, std::string attrHeader) override;
	};

	class Statement : public DesignEntity {
	public:
		Statement();
		synonym_values getSynonymFromPKB(GetFacade* facade) override;
		synonym_attrRef getSynonymWithAttrRefFromPKB(GetFacade* facade, std::string synHeader, std::string attrHeader) override;
	};

	class Read : public DesignEntity {
	public:
		Read();
		synonym_values getSynonymFromPKB(GetFacade* facade) override;
		synonym_attrRef getSynonymWithAttrRefFromPKB(GetFacade* facade, std::string synHeader, std::string attrHeader) override;
	};

	class Print : public DesignEntity {
	public:
		Print();
		synonym_values getSynonymFromPKB(GetFacade* facade) override;
		synonym_attrRef getSynonymWithAttrRefFromPKB(GetFacade* facade, std::string synHeader, std::string attrHeader) override;
	};

	class Assign : public DesignEntity {
	public:
		Assign();
		synonym_values getSynonymFromPKB(GetFacade* facade) override;
		synonym_attrRef getSynonymWithAttrRefFromPKB(GetFacade* facade, std::string synHeader, std::string attrHeader) override;
	};

	class Call : public DesignEntity {
	public:
		Call();
		synonym_values getSynonymFromPKB(GetFacade* facade) override;
		synonym_attrRef getSynonymWithAttrRefFromPKB(GetFacade* facade, std::string synHeader, std::string attrHeader) override;
	};

	class While : public DesignEntity {
	public:
		While();
		synonym_values getSynonymFromPKB(GetFacade* facade) override;
		synonym_attrRef getSynonymWithAttrRefFromPKB(GetFacade* facade, std::string synHeader, std::string attrHeader) override;
	};

	class If : public DesignEntity {
	public:
		If();
		synonym_values getSynonymFromPKB(GetFacade* facade) override;
		synonym_attrRef getSynonymWithAttrRefFromPKB(GetFacade* facade, std::string synHeader, std::string attrHeader) override;
	};

	class Variable : public DesignEntity {
	public:
		Variable();
		synonym_values getSynonymFromPKB(GetFacade* facade) override;
		synonym_attrRef getSynonymWithAttrRefFromPKB(GetFacade* facade, std::string synHeader, std::string attrHeader) override;
	};

	class Constant : public DesignEntity {
	public:
		Constant();
		synonym_values getSynonymFromPKB(GetFacade* facade) override;
		synonym_attrRef getSynonymWithAttrRefFromPKB(GetFacade* facade, std::string synHeader, std::string attrHeader) override;
	};
}

