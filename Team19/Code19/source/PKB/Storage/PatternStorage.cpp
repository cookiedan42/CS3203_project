#pragma once

#include "PatternStorage.h"
#include <iostream>
#include <algorithm>

PatternStorage::PatternStorage() {}

void PatternStorage::insertAssign(Ptn::AssignPtn pattern) {
	assignStorage.insertPattern(pattern);
}

void PatternStorage::insertWhile(Ptn::WhilePtn pattern) {
	whileStorage.insertPattern(pattern);
}

void PatternStorage::insertIf(Ptn::IfPtn pattern) {
	ifStorage.insertPattern(pattern);
}

AssignPatternStorage PatternStorage::getAssignStorage() {
	return assignStorage;
}

IfPatternStorage PatternStorage::getIfStorage() {
	return ifStorage;
}

WhilePatternStorage PatternStorage::getWhileStorage() {
	return whileStorage;
}