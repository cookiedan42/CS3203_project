#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <variant>

#include "Declaration.h"
#include "Variants/StmtNo.h"
#include "Variants/Integer.h"
#include "Variants/StringExp.h"
#include "Variants/SubExp.h"
#include "Variants/WildCard.h"

struct SynAttr {
	Declaration synonym;
	ArgType attributeType;

	bool SynAttr::operator==(const SynAttr& synAttr) const {
		return tie(synonym, attributeType) == tie(synAttr.synonym, synAttr.attributeType);
	}
};

struct Argument {
	using Arg = std::variant<std::monostate, Declaration, StmtNo, StringExp, SubExp, WildCard, SynAttr, Integer>;
	Arg argumentVariant;

	bool operator==(const Argument& otherArg) const {
		if (this == &otherArg) { return true; } // if same obj
		size_t thisArgIdx = argumentVariant.index();
		size_t otherArgIdx = otherArg.argumentVariant.index();
		if (thisArgIdx != otherArgIdx) { return false; } // diff variant types

		// if same variant type, check if contents are the same
		return visit([](const Arg& thisArg, const Arg& otherArg) {
			return thisArg == otherArg;
			}, this->argumentVariant, otherArg.argumentVariant);
	}
};