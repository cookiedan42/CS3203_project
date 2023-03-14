#pragma once

#include "../../Pattern/Pattern.h"
#include "PatternStorages/AssignPatternStorage.h"
#include "PatternStorages/IfPatternStorage.h"
#include "PatternStorages/WhilePatternStorage.h"
#include <unordered_map>
#include <unordered_set>
#include <algorithm> //std::sort
#include <string> //std::string
#include <iterator>


class PatternStorage {
public:
	PatternStorage();

	void insertAssign(Ptn::AssignPtn);
	void insertWhile(Ptn::WhilePtn);
	void insertIf(Ptn::IfPtn);

	AssignPatternStorage getAssignStorage();
	WhilePatternStorage getWhileStorage();
	IfPatternStorage getIfStorage();

private:		

	AssignPatternStorage assignStorage;
	IfPatternStorage ifStorage;
	WhilePatternStorage whileStorage;
};