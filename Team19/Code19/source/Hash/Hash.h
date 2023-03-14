#pragma once

#include <functional>
#include <utility>

/*
 
 Template: https://stackoverflow.com/questions/9729390/how-to-use-unordered-set-with-custom-types/9729747#9729747
 Szudzik's function: a >= b ? a * a + a + b : a + b * b: http://szudzik.com/ElegantPairing.pdf
*/

struct stringPairHash {
public:
	template <typename U, typename T> size_t operator()(const std::pair<U, T>& v) const {
		std::hash<std::string> strHasher;
		auto a = strHasher(v.first);
		auto b = strHasher(v.second);
		return a >= b ? a * a + a + b : a + b * b;
	}
};


struct argTypePairHash {
public:
	template <typename U, typename T> size_t operator()(const std::pair<U, T>& v) const {
		std::hash<ArgType> argTypeHasher;
		auto a = argTypeHasher(v.first);
		auto b = argTypeHasher(v.second);
		return a >= b ? a * a + a + b : a + b * b;
	}
};

