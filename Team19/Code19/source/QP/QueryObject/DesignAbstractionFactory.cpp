#include "DesignAbstractionFactory.h"


/*
* Create design abstraction object.
* 
* Note: If-Else is needed to create the relevant design abstraction object.
*/
shared_ptr<DesignAbstraction> DesignAbstractionFactory::createDesignAbstraction(std::string relationStr, Argument arg1, Argument arg2)
{
	if (relationStr == FOLLOWS_REL)
	{
		return make_shared<DA::FollowsRel>(arg1, arg2);
	}
	else if (relationStr == FOLLOWS_T_REL)
	{
		return make_shared<DA::FollowsTRel>(arg1, arg2);
	}
	else if (relationStr == PARENT_REL)
	{
		return make_shared<DA::ParentRel>(arg1, arg2);
	}
	else if (relationStr == PARENT_T_REL)
	{
		return make_shared<DA::ParentTRel>(arg1, arg2);
	}
	else if (relationStr == MODIFIES_REL)
	{
		return make_shared<DA::ModifiesRel>(arg1, arg2);
	}
	else if (relationStr == USES_REL)
	{
		return make_shared<DA::UsesRel>(arg1, arg2);
	}
	else if (relationStr == CALLS_REL)
	{
		return make_shared<DA::CallsRel>(arg1, arg2);
	}
	else if (relationStr == CALLS_T_REL)
	{
		return make_shared<DA::CallsTRel>(arg1, arg2);
	}
	else if (relationStr == NEXT_REL)
	{
		return make_shared<DA::NextRel>(arg1, arg2);
	}
	else if (relationStr == NEXT_T_REL)
	{
		return make_shared<DA::NextTRel>(arg1, arg2);
	}
	else if (relationStr == AFFECTS_REL)
	{
		return make_shared<DA::AffectsRel>(arg1, arg2);
	}
	else if (relationStr == AFFECTS_T_REL)
	{
		return make_shared<DA::AffectsTRel>(arg1, arg2);
	}
	else {
		throw QuerySyntaxException("");
	}
}