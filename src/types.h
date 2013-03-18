/*
 * types.h
 *
 *  Created on: 17 mars 2013
 *      Author: quentinfiard
 */

#ifndef TYPES_H_
#define TYPES_H_

#include <stdint.h>
#include <vector>

typedef int32_t literal;

typedef struct {
	uint32_t length;
	literal* literals;
} clause;

typedef struct {
	uint32_t nbOfClauses;
	uint32_t initialNbOfClauses;
	uint32_t nbOfVariables;
	clause* clauses;
} formula;

typedef std::vector<int8_t> assignment;

class SAT : public std::exception
{
private:
	assignment a;
public:
	SAT(assignment &a) : a(a)
	{

	}

	assignment& getAssigment()
	{
		return a;
	}
};

class UNSAT : public std::exception
{

};


#endif /* TYPES_H_ */
