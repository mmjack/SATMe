#ifndef _CLAUSE_DEF_H_
#define _CLAUSE_DEF_H_
#include "clause_partial.h"

/**
 * In a 3-sat a clause is the the disjunction of 3 clause partials (Variable or ¬Variable)
 */
typedef struct Clause {
	ClausePartial A;
	ClausePartial B;
	ClausePartial C;
} Clause;

#endif //_CLAUSE_DEF_H_