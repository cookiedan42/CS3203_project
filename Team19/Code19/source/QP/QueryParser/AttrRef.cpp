#include "./AttrRef.h"

bool AttrRef::isNameType() {
	return std::find(NAME_T_ATTRNAMES.cbegin(), 
		NAME_T_ATTRNAMES.cend(), attrName) 
		!= NAME_T_ATTRNAMES.cend();
}
bool AttrRef::isIntType() {
	return std::find(INT_T_ATTRNAMES.cbegin(),
		INT_T_ATTRNAMES.cend(), attrName)
		!= INT_T_ATTRNAMES.cend();
}