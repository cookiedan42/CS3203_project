#include "Declaration.h"

Declaration::Declaration()
{}

Declaration::Declaration(shared_ptr<DesignEntity> type, std::string name) 
	: synType(type), synName(name) 
{}

bool Declaration::operator==(const Declaration& dec) const {
	return tie(synName) == tie(dec.synName) && typeid(synType) == typeid(dec.synType);
}

bool Declaration::operator!=(const Declaration& dec) const {
	return (tie(synName) != tie(dec.synName)) || (typeid(synType) != typeid(dec.synType));
}
