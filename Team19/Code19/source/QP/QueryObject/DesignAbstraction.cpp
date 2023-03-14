#include "DesignAbstraction.h"
#include "../QueryEvaluator/FollowsEvaluator.h"
#include "../QueryEvaluator/FollowsTEvaluator.h"
#include "../QueryEvaluator/ParentEvaluator.h"
#include "../QueryEvaluator/ParentTEvaluator.h"
#include "../QueryEvaluator/ModifiesEvaluator.h"
#include "../QueryEvaluator/UsesEvaluator.h"
#include "../QueryEvaluator/CallsEvaluator.h"
#include "../QueryEvaluator/CallsTEvaluator.h"
#include "../QueryEvaluator/NextEvaluator.h"
#include "../QueryEvaluator/NextTEvaluator.h"
#include "../QueryEvaluator/AffectsEvaluator.h"
#include "../QueryEvaluator/AffectsTEvaluator.h"

using namespace DA;

bool DesignAbstraction::evaluateClauseWithNoSyn(GetFacade* facade)
{
	return false;
}

bool DesignAbstraction::evaluateClauseWithSynToBool(GetFacade* facade)
{
	return false;
}

Table DesignAbstraction::evaluateClauseWithSyn(GetFacade* facade)
{
	return Table();
}


bool DesignAbstraction::operator==(const DesignAbstraction& da) const
{
	return tie(type, arg1, arg2) == tie(da.type, da.arg1, da.arg2);
}

bool DesignAbstraction::operator!=(const DesignAbstraction& da) const
{
	return tie(type, arg1, arg2) != tie(da.type, da.arg1, da.arg2);
}

/* FollowsRel constructor and methods */
DA::FollowsRel::FollowsRel(Argument arg1, Argument arg2)
{
	this->type = FOLLOWS_REL;
	this->arg1 = arg1;
	this->arg2 = arg2;
	this->relationshipPenalty = 0;
}

bool DA::FollowsRel::evaluateClauseWithNoSyn(GetFacade* facade)
{
	return FollowsEvaluator(this->arg1, this->arg2, *facade).evaluateClauseWithNoSyn();
}

bool DA::FollowsRel::evaluateClauseWithSynToBool(GetFacade* facade)
{
	return FollowsEvaluator(this->arg1, this->arg2, *facade).evaluateClauseWithSynToBool();
}

Table DA::FollowsRel::evaluateClauseWithSyn(GetFacade* facade)
{
	return FollowsEvaluator(this->arg1, this->arg2, *facade).evaluateClauseWithSyn();
}

/* FollowsTRel constructor and methods */
DA::FollowsTRel::FollowsTRel(Argument arg1, Argument arg2)
{
	this->type = FOLLOWS_T_REL;
	this->arg1 = arg1;
	this->arg2 = arg2;
	this->relationshipPenalty = STAR_PENALTY;
}

bool DA::FollowsTRel::evaluateClauseWithNoSyn(GetFacade* facade)
{
	return FollowsTEvaluator(this->arg1, this->arg2, *facade).evaluateClauseWithNoSyn();
}

bool DA::FollowsTRel::evaluateClauseWithSynToBool(GetFacade* facade)
{
	return FollowsTEvaluator(this->arg1, this->arg2, *facade).evaluateClauseWithSynToBool();
}

Table DA::FollowsTRel::evaluateClauseWithSyn(GetFacade* facade)
{
	return FollowsTEvaluator(this->arg1, this->arg2, *facade).evaluateClauseWithSyn();
}


/* ParentRel constructor and methods */
DA::ParentRel::ParentRel(Argument arg1, Argument arg2)
{
	this->type = PARENT_REL;
	this->arg1 = arg1;
	this->arg2 = arg2;
	this->relationshipPenalty = 0;
}

bool DA::ParentRel::evaluateClauseWithNoSyn(GetFacade* facade)
{
	return ParentEvaluator(this->arg1, this->arg2, *facade).evaluateClauseWithNoSyn();
}

bool DA::ParentRel::evaluateClauseWithSynToBool(GetFacade* facade)
{
	return ParentEvaluator(this->arg1, this->arg2, *facade).evaluateClauseWithSynToBool();
}

Table DA::ParentRel::evaluateClauseWithSyn(GetFacade* facade)
{
	return ParentEvaluator(this->arg1, this->arg2, *facade).evaluateClauseWithSyn();
}


/* ParentTRel constructor and methods */
DA::ParentTRel::ParentTRel(Argument arg1, Argument arg2)
{
	this->type = PARENT_T_REL;
	this->arg1 = arg1;
	this->arg2 = arg2;
	this->relationshipPenalty = STAR_PENALTY;
}

bool DA::ParentTRel::evaluateClauseWithNoSyn(GetFacade* facade)
{
	return ParentTEvaluator(this->arg1, this->arg2, *facade).evaluateClauseWithNoSyn();
}

bool DA::ParentTRel::evaluateClauseWithSynToBool(GetFacade* facade)
{
	return ParentTEvaluator(this->arg1, this->arg2, *facade).evaluateClauseWithSynToBool();
}

Table DA::ParentTRel::evaluateClauseWithSyn(GetFacade* facade)
{
	return ParentTEvaluator(this->arg1, this->arg2, *facade).evaluateClauseWithSyn();
}


/* ModifiesRel constructor and methods */
DA::ModifiesRel::ModifiesRel(Argument arg1, Argument arg2)
{
	this->type = MODIFIES_REL;
	this->arg1 = arg1;
	this->arg2 = arg2;
	this->relationshipPenalty = 0;
}

bool DA::ModifiesRel::evaluateClauseWithNoSyn(GetFacade* facade)
{
	return ModifiesEvaluator(this->arg1, this->arg2, *facade).evaluateClauseWithNoSyn();
}

bool DA::ModifiesRel::evaluateClauseWithSynToBool(GetFacade* facade)
{
	return ModifiesEvaluator(this->arg1, this->arg2, *facade).evaluateClauseWithNoSyn();
}

Table DA::ModifiesRel::evaluateClauseWithSyn(GetFacade* facade)
{
	return ModifiesEvaluator(this->arg1, this->arg2, *facade).evaluateClauseWithSyn();
}

/* UsesRel constructor and methods */
DA::UsesRel::UsesRel(Argument arg1, Argument arg2)
{
	this->type = USES_REL;
	this->arg1 = arg1;
	this->arg2 = arg2;
	this->relationshipPenalty = 0;
}

bool DA::UsesRel::evaluateClauseWithNoSyn(GetFacade* facade)
{
	return UsesEvaluator(this->arg1, this->arg2, *facade).evaluateClauseWithNoSyn();
}

bool DA::UsesRel::evaluateClauseWithSynToBool(GetFacade* facade)
{
	return UsesEvaluator(this->arg1, this->arg2, *facade).evaluateClauseWithSynToBool();
}

Table DA::UsesRel::evaluateClauseWithSyn(GetFacade* facade)
{
	return UsesEvaluator(this->arg1, this->arg2, *facade).evaluateClauseWithSyn();
}

/* CallsRel constructor and methods */
DA::CallsRel::CallsRel(Argument arg1, Argument arg2)
{
	this->type = CALLS_REL;
	this->arg1 = arg1;
	this->arg2 = arg2;
	this->relationshipPenalty = 0;
}

bool DA::CallsRel::evaluateClauseWithNoSyn(GetFacade* facade)
{
	return CallsEvaluator(this->arg1, this->arg2, *facade).evaluateClauseWithNoSyn();
}

bool DA::CallsRel::evaluateClauseWithSynToBool(GetFacade* facade)
{
	return CallsEvaluator(this->arg1, this->arg2, *facade).evaluateClauseWithSynToBool();
}


Table DA::CallsRel::evaluateClauseWithSyn(GetFacade* facade)
{
	return CallsEvaluator(this->arg1, this->arg2, *facade).evaluateClauseWithSyn();
}

/* CallsTRel constructor and methods */
DA::CallsTRel::CallsTRel(Argument arg1, Argument arg2)
{
	this->type = CALLS_T_REL;
	this->arg1 = arg1;
	this->arg2 = arg2;
	this->relationshipPenalty = STAR_PENALTY;
}

bool DA::CallsTRel::evaluateClauseWithNoSyn(GetFacade* facade)
{
	return CallsTEvaluator(this->arg1, this->arg2, *facade).evaluateClauseWithNoSyn();
}

bool DA::CallsTRel::evaluateClauseWithSynToBool(GetFacade* facade)
{
	return CallsTEvaluator(this->arg1, this->arg2, *facade).evaluateClauseWithSynToBool();
}

Table DA::CallsTRel::evaluateClauseWithSyn(GetFacade* facade)
{
	return CallsTEvaluator(this->arg1, this->arg2, *facade).evaluateClauseWithSyn();
}


/* NextRel constructor and methods */
DA::NextRel::NextRel(Argument arg1, Argument arg2)
{
	this->type = NEXT_REL;
	this->arg1 = arg1;
	this->arg2 = arg2;
	this->relationshipPenalty = 0;
}

bool DA::NextRel::evaluateClauseWithNoSyn(GetFacade* facade)
{
	return NextEvaluator(this->arg1, this->arg2, *facade).evaluateClauseWithNoSyn();
}

bool DA::NextRel::evaluateClauseWithSynToBool(GetFacade* facade)
{
	return NextEvaluator(this->arg1, this->arg2, *facade).evaluateClauseWithSynToBool();
}

Table DA::NextRel::evaluateClauseWithSyn(GetFacade* facade)
{
	return NextEvaluator(this->arg1, this->arg2, *facade).evaluateClauseWithSyn();
}


/* NextTRel constructor and methods */
DA::NextTRel::NextTRel(Argument arg1, Argument arg2)
{
	this->type = NEXT_T_REL;
	this->arg1 = arg1;
	this->arg2 = arg2;
	this->relationshipPenalty = STAR_PENALTY + RUNTIME_PENALTY;
}

bool DA::NextTRel::evaluateClauseWithNoSyn(GetFacade* facade)
{
	return NextTEvaluator(this->arg1, this->arg2, *facade).evaluateClauseWithNoSyn();
}

bool DA::NextTRel::evaluateClauseWithSynToBool(GetFacade* facade)
{
	return NextTEvaluator(this->arg1, this->arg2, *facade).evaluateClauseWithSynToBool();
}

Table DA::NextTRel::evaluateClauseWithSyn(GetFacade* facade)
{
	return NextTEvaluator(this->arg1, this->arg2, *facade).evaluateClauseWithSyn();
}

/* AffectsRel constructor and methods */
DA::AffectsRel::AffectsRel(Argument arg1, Argument arg2)
{
	this->type = AFFECTS_REL;
	this->arg1 = arg1;
	this->arg2 = arg2;
	this->relationshipPenalty = AFFECTS_PENALTY + RUNTIME_PENALTY;
}

bool DA::AffectsRel::evaluateClauseWithNoSyn(GetFacade* facade)
{
	return AffectsEvaluator(this->arg1, this->arg2, *facade).evaluateClauseWithNoSyn();
}

bool DA::AffectsRel::evaluateClauseWithSynToBool(GetFacade* facade)
{
	return AffectsEvaluator(this->arg1, this->arg2, *facade).evaluateClauseWithSynToBool();
}

Table DA::AffectsRel::evaluateClauseWithSyn(GetFacade* facade)
{
	return AffectsEvaluator(this->arg1, this->arg2, *facade).evaluateClauseWithSyn();
}

/* AffectsRel constructor and methods */
DA::AffectsTRel::AffectsTRel(Argument arg1, Argument arg2)
{
	this->type = AFFECTS_T_REL;
	this->arg1 = arg1;
	this->arg2 = arg2;
	this->relationshipPenalty = AFFECTS_PENALTY + STAR_PENALTY + RUNTIME_PENALTY;
}

bool DA::AffectsTRel::evaluateClauseWithNoSyn(GetFacade* facade)
{
	return AffectsTEvaluator(this->arg1, this->arg2, *facade).evaluateClauseWithNoSyn();
}

bool DA::AffectsTRel::evaluateClauseWithSynToBool(GetFacade* facade)
{
	return AffectsTEvaluator(this->arg1, this->arg2, *facade).evaluateClauseWithSynToBool();
}

Table DA::AffectsTRel::evaluateClauseWithSyn(GetFacade* facade)
{
	return AffectsTEvaluator(this->arg1, this->arg2, *facade).evaluateClauseWithSyn();
}