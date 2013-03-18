/*
 * types.h
 *
 *  Created on: 17 mars 2013
 *      Author: quentinfiard
 */

#ifndef TYPES_H_
#define TYPES_H_

#include "config.h"
#include <stdint.h>
#include <string>
#include <vector>
#include <map>

typedef int32_t literal;

typedef struct {
	uint32_t length;
	literal* literals;
} clause;

typedef struct {
    std::string source_file;
	uint32_t nbOfClauses;
	uint32_t initialNbOfClauses;
	uint32_t nbOfVariables;
	clause* clauses;
} formula;

typedef std::vector<int8_t> assignment;


bool compare(formula f1, formula f2);

clause copy(clause c);
formula copy(formula& f);
assignment copy(assignment& a);
clause deepcopy(clause c);
formula deepcopy(formula& f);
assignment deepcopy(assignment& a);
void dealloc(clause& c);
void dealloc(formula& f);
void deep_dealloc(clause& c);
void deep_dealloc(formula& f);


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

typedef std::map<std::string, double> time_map;

typedef struct {
    std::string name;
    double time_elapsed;
    time_map time_for_function;
} benchmark_data;

typedef std::string path;
typedef std::vector<path> paths;

#endif /* TYPES_H_ */
