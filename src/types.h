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
#include <cstring>
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

typedef struct {
	bool unsat;
	uint32_t length;
	literal* literals;
} assignment;

bool compare(formula f1, formula f2);

clause copy(clause& c);
assignment copy(assignment& a);
formula copy(formula& f);
clause deepcopy(clause& c);
assignment deepcopy(assignment& a);
formula deepcopy(formula& f);
void dealloc(clause& c);
void dealloc(assignment& a);
void dealloc(formula& f);
void deep_dealloc(clause& c);
void deep_dealloc(assignment& c);
void deep_dealloc(formula& f);

#ifdef __cplusplus

class SAT : public std::exception
{
private:
	assignment a;
public:
	SAT(assignment &a)
	{
		this->a = deepcopy(a);
	}

	SAT(SAT &e) : std::exception(e)
	{
		this->a = deepcopy(e.a);
	}

	virtual ~SAT() throw ()
	{
        deep_dealloc(a);
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

#endif

#endif /* TYPES_H_ */
