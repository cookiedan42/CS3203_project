#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <unordered_set>
#include <memory>
#include "../../PKB/GetFacade.h"
#include "../QueryEvaluator/QueryUtil.h"
#include "Argument.h"

using namespace std;


class DesignAbstraction {
public:
	std::string type;
	Argument arg1 = { std::monostate{} };
	Argument arg2 = { std::monostate{} };

	int relationshipPenalty;

	DesignAbstraction() = default;

	bool operator==(const DesignAbstraction& da) const;
	bool operator!=(const DesignAbstraction& da) const;

	virtual bool evaluateClauseWithNoSyn(GetFacade* facade);
	virtual bool evaluateClauseWithSynToBool(GetFacade* facade);
	virtual Table evaluateClauseWithSyn(GetFacade* facade);
};



namespace DA {
	class FollowsRel : public DesignAbstraction {
	public:
		FollowsRel(Argument arg1, Argument arg2);

		bool evaluateClauseWithNoSyn(GetFacade* pkb) override;
		bool evaluateClauseWithSynToBool(GetFacade* pkb) override;
		Table evaluateClauseWithSyn(GetFacade* pkb) override;
	};

	class FollowsTRel : public DesignAbstraction {
	public:
		FollowsTRel(Argument arg1, Argument arg2);
		bool evaluateClauseWithNoSyn(GetFacade* pkb) override;
		bool evaluateClauseWithSynToBool(GetFacade* pkb) override;
		Table evaluateClauseWithSyn(GetFacade* pkb) override;
	};

	class ParentRel : public DesignAbstraction {
	public:
		ParentRel(Argument arg1, Argument arg2);
		bool evaluateClauseWithNoSyn(GetFacade* pkb) override;
		bool evaluateClauseWithSynToBool(GetFacade* pkb) override;
		Table evaluateClauseWithSyn(GetFacade* pkb) override;
	};

	class ParentTRel : public DesignAbstraction {
	public:
		ParentTRel(Argument arg1, Argument arg2);
		bool evaluateClauseWithNoSyn(GetFacade* pkb) override;
		bool evaluateClauseWithSynToBool(GetFacade* pkb) override;
		Table evaluateClauseWithSyn(GetFacade* pkb) override;
	};

	class ModifiesRel : public DesignAbstraction {
	public:
		ModifiesRel(Argument arg1, Argument arg2);
		bool evaluateClauseWithNoSyn(GetFacade* pkb) override;
		bool evaluateClauseWithSynToBool(GetFacade* pkb) override;
		Table evaluateClauseWithSyn(GetFacade* pkb) override;
	};

	class UsesRel : public DesignAbstraction {
	public:
		UsesRel(Argument arg1, Argument arg2);
		bool evaluateClauseWithNoSyn(GetFacade* pkb) override;
		bool evaluateClauseWithSynToBool(GetFacade* pkb) override;
		Table evaluateClauseWithSyn(GetFacade* pkb) override;
	};

	class CallsRel : public DesignAbstraction {
	public:
		CallsRel(Argument arg1, Argument arg2);
		bool evaluateClauseWithNoSyn(GetFacade* pkb) override;
		bool evaluateClauseWithSynToBool(GetFacade* pkb) override;
		Table evaluateClauseWithSyn(GetFacade* pkb) override;
	};

	class CallsTRel : public DesignAbstraction {
	public:
		CallsTRel(Argument arg1, Argument arg2);
		bool evaluateClauseWithNoSyn(GetFacade* pkb) override;
		bool evaluateClauseWithSynToBool(GetFacade* pkb) override;
		Table evaluateClauseWithSyn(GetFacade* pkb) override;
	};

	class NextRel : public DesignAbstraction {
	public:
		NextRel(Argument arg1, Argument arg2);
		bool evaluateClauseWithNoSyn(GetFacade* pkb) override;
		bool evaluateClauseWithSynToBool(GetFacade* pkb) override;
		Table evaluateClauseWithSyn(GetFacade* pkb) override;
	};

	class NextTRel : public DesignAbstraction {
	public:
		NextTRel(Argument arg1, Argument arg2);
		bool evaluateClauseWithNoSyn(GetFacade* pkb) override;
		bool evaluateClauseWithSynToBool(GetFacade* pkb) override;
		Table evaluateClauseWithSyn(GetFacade* pkb) override;
	};

	class AffectsRel : public DesignAbstraction {
	public:
		AffectsRel(Argument arg1, Argument arg2);
		bool evaluateClauseWithNoSyn(GetFacade* pkb) override;
		bool evaluateClauseWithSynToBool(GetFacade* pkb) override;
		Table evaluateClauseWithSyn(GetFacade* pkb) override;
	};

	class AffectsTRel : public DesignAbstraction {
	public:
		AffectsTRel(Argument arg1, Argument arg2);
		bool evaluateClauseWithNoSyn(GetFacade* pkb) override;
		bool evaluateClauseWithSynToBool(GetFacade* pkb) override;
		Table evaluateClauseWithSyn(GetFacade* pkb) override;
	};

}

