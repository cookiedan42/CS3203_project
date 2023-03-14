#pragma once
#include <string>
#include <variant>
#include <memory>

// propogate the use of unique_ptr
// without the rest of standard library
using std::unique_ptr;

auto compareStatements = [](const auto& thisStmt, const auto& otherStmt) {
	return visit([](const auto& ts, const auto& os) {
		return *ts == *os;
		}, thisStmt, otherStmt);
};

auto compareExpression = [](const auto& thisExpr, const auto& otherExpr) {
	return *thisExpr == *otherExpr;
};

auto compareRelationalFactor = [](const auto& thisFactor, const auto& otherFactor) {
	return *thisFactor == *otherFactor;
};

namespace SP {
	struct Node {
		virtual bool operator==(const Node& other) const = 0;
		bool operator!=(const Node& other) const {
			return !(*this == other);
		};
	};
}