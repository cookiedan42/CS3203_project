#pragma once

#include <vector>
#include <string>

namespace SIMPLE {
	// Token type
	constexpr char* NAME = "name";
	constexpr char* INTEGER = "integer";
	constexpr char* SYMBOL = "symbol";

	// Concrete Syntax Grammar - For parser/validation
	constexpr char* PROCEDURE = "procedure";
	constexpr char* READ = "read";
	constexpr char* CALL = "call";
	constexpr char* PRINT = "print";
	constexpr char* IF = "if";
	constexpr char* THEN = "then";
	constexpr char* ELSE = "else";
	constexpr char* WHILE = "while";
	constexpr char* EQUALS = "=";
	constexpr char* LEFT_CURLY = "{";
	constexpr char* RIGHT_CURLY = "}";
	constexpr char* LEFT_BRACKET = "(";
	constexpr char* RIGHT_BRACKET = ")";
	constexpr char* EOL = ";";
	constexpr char* NOT = "!";
	constexpr char* AND = "&&";
	constexpr char* OR = "||";

	// Expression Parser
	constexpr int DEFAULT_PRECEDENCE = -1;
	constexpr int PLUS_PRECEDENCE = 1;
	constexpr int MINUS_PRECEDENCE = 1;
	constexpr int MULTIPLY_PRECEDENCE = 2;
	constexpr int DIVIDE_PRECEDENCE = 2;
	constexpr int MODULO_PRECEDENCE = 2;
	constexpr char* PLUS_OPERATOR = "+";
	constexpr char* MINUS_OPERATOR = "-";
	constexpr char* MULTIPLY_OPERATOR = "*";
	constexpr char* DIVIDE_OPERATOR = "/";
	constexpr char* MODULO_OPERATOR = "%";
	constexpr char* LEFT_PARENTHESIS = "(";
	constexpr char* RIGHT_PARENTHESIS = ")";

}

namespace Exception {
	constexpr char* ITERATOR_OUT_OF_RANGE = "TokenIterator out of range";

	constexpr char* INVALID_INTEGER = "Invalid integer";
	constexpr char* INVALID_NAME = "Invalid name";
	
	constexpr char* EMPTY_SOURCE = "Source code cannot be empty";
	constexpr char* EMPTY_STATEMENT_LIST = "Statement list cannot be empty";

	constexpr char* INVALID_SYNTAX_GRAMMAR = "Invalid syntax grammar";
	constexpr char* SEMANTIC_ERROR = "Semantic error found";
}

namespace Ent {
	constexpr char* const_entityType = "constant";
	constexpr char* var_entityType = "variable";
	constexpr char* proc_entityType = "procedure";
	constexpr char* stmt_entityType = "statement";

	constexpr char* read_stmtType = "read";
	constexpr char* print_stmtType = "print";
	constexpr char* assign_stmtType = "assignment";
	constexpr char* call_stmtType = "call";
	constexpr char* while_stmtType = "while";
	constexpr char* if_stmtType = "if";
}
//For relationship requests
#define EMPTY_ARG ""

//For relationship objects
#define FOLLOWS "follows"
#define FOLLOWS_STAR "followsStar"
#define PARENT "parent"
#define PARENT_STAR "parentStar"
#define MODIFIES "modifies"
#define USES "uses"
#define CALLS "calls"
#define CALLS_STAR "callsStar"
#define NEXT "next"
#define NEXT_STAR "nextStar"
#define DEFAULT_RS "default"
#define UNKNOWN "unknown"

//Star booleans
#define STAR true
#define NO_STAR false

//For Pattern Objects
#define READTYPE "read"
#define PRINTTYPE "print"
#define ASSIGNMENTTYPE "assignment"
#define CALLTYPE "call"
#define WHILETYPE "while"
#define IFTYPE "if"
#define CONSTANTTYPE "constant"
#define VARIABLETYPE "variable"
#define STATEMENTTYPE "statement"
#define PROCEDURETYPE "procedure"
const std::vector<std::string> STMT_TYPES = { READTYPE, PRINTTYPE, ASSIGNMENTTYPE, CALLTYPE, WHILETYPE, IFTYPE,STATEMENTTYPE };


//For splitting
#define ATTR_STR_DELIM "."
#define SELECT_KEYWORD "Select"
#define SUCH_THAT_FIRST_KEYWORD "such"
#define SUCH_THAT_SCND_KEYWORD "that"
#define PATTERN_KEYWORD "pattern"
#define WITH_KEYWORD "with"
#define AND_KEYWORD "and"
#define BOOL_KEYWORD "BOOLEAN"
#define DEC_DELIMITER ";"
#define DEC_MULT_VAR_DELIMITER ","
#define TUPLE_START_DELIMITER "<"
#define TUPLE_END_DELIMITER ">"
#define BRACKET_START_DELIMITER "("
#define BRACKET_END_DELIMITER ")"
#define REL_STAR_DELIM "*"
#define STMTNO_KEYWORD "stmt"
#define STMTNO_HASH "#"

//For design-entities
#define STATEMENT_DE "stmt"
#define READ_DE "read"
#define PRINT_DE "print"
#define CALL_DE "call"
#define WHILE_DE "while"
#define IF_DE "if"
#define ASSIGN_DE "assign"
#define VARIABLE_DE "variable"
#define CONSTANT_DE "constant"
#define PROCEDURE_DE "procedure"
const std::vector<std::string> DE_TYPES = { STATEMENT_DE, READ_DE, PRINT_DE, CALL_DE, WHILE_DE, IF_DE, ASSIGN_DE, VARIABLE_DE, CONSTANT_DE, PROCEDURE_DE };

//For parsing
#define STRING_DELIM '"'
#define SELECT_DEC_DELIM ','
#define ATTR_DELIM '.'
#define COMPARE_DELIM '='
#define WILDCARD_DELIM "_"

//For CFG requests
#define EMPTY_STMTNO -1
#define EMPTY_PROCNAME ""

//For parsing such that clause
#define FOLLOWS_REL "Follows"
#define FOLLOWS_T_REL "Follows*"
#define PARENT_REL "Parent"
#define PARENT_T_REL "Parent*"
#define USES_REL "Uses"
#define MODIFIES_REL "Modifies"
#define CALLS_REL "Calls"
#define CALLS_T_REL "Calls*"
#define NEXT_REL "Next"
#define NEXT_T_REL "Next*"
#define AFFECTS_REL "Affects"
#define AFFECTS_T_REL "Affects*"
const std::vector<std::string> STMT_PAIR_REL = { 
	FOLLOWS_REL, FOLLOWS_T_REL,
	PARENT_REL, PARENT_T_REL,
	NEXT_REL, NEXT_T_REL, 
	AFFECTS_REL, AFFECTS_T_REL};


//For parsing with clause
#define PROC_NAME_ATTRNAME "procName"
#define VAR_NAME_ATTRNAME "varName"
#define VALUE_ATTRNAME "value"
#define STMTNO_ATTRNAME "stmt#"


//For synonym attribute
#define REF_INT_STR ".ref_int"
#define REF_INT_NAME ".ref_name"
const std::vector<std::string> NAME_T_ATTRNAMES = { PROC_NAME_ATTRNAME, VAR_NAME_ATTRNAME };
const std::vector<std::string> INT_T_ATTRNAMES = { VALUE_ATTRNAME, STMTNO_ATTRNAME };


enum class ArgType {
	SYN,
	WILDCARD,
	STMTNO,
	QUOTE,
	SUBQUOTE,
	EMPTY,
	ATTR_REF_INT,
	ATTR_REF_NAME,
	INTEGER,
};

enum class ArgsType {
	SYN_SYN,
	SYN_WILDCARD,
	SYN_STMTNO,
	SYN_QUOTE,
	SYN_SUBQUOTE,

	WILDCARD_SYN,
	WILDCARD_STMTNO,
	WILDCARD_WILDCARD,
	WILDCARD_QUOTE,
	WILDCARD_SUBQUOTE,

	STMTNO_SYN,
	STMTNO_WILDCARD,
	STMTNO_STMTNO,
	STMTNO_QUOTE,

	QUOTE_WILDCARD,
	QUOTE_QUOTE,
	QUOTE_SUBQUOTE,
	QUOTE_SYN,

	// Variant with clause
	INTEGER_INTEGER,
	REFINT_INTEGER,
	INTEGER_REFINT,
	REFNAME_QUOTE,
	QUOTE_REFNAME,
	REFINT_REFINT,
	REFNAME_REFNAME,
	EMPTY

};

#define WITH_CLAUSE_PENALTY 500
#define SUCH_THAT_CLAUSE_PENALTY 500
#define PATTERN_CLAUSE_PENALTY 500
#define SYN_PENALTY 500

#define STAR_PENALTY 1000
#define RUNTIME_PENALTY 2000
#define AFFECTS_PENALTY 10000

// For CFG
#define PROC_END 0