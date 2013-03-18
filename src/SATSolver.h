/*
 * SATSolver.h
 *
 *  Created on: 17 mars 2013
 *      Author: quentinfiard
 */

#ifndef SATSOLVER_H_
#define SATSOLVER_H_

#include "types.h"
#include <map>

class SATSolver {
private:
	static void check_sat_given_partial_assignment(formula& f, assignment& partial, int level);

	static void unit_propagate(formula& f, assignment& partial);
	static void process_unit_clauses(formula& f, assignment& partial);
	static void assign_pure_literals(formula& f, assignment& partial);

	static void remove_literal_from_clause(uint32_t index_to_remove, clause &c);
	static void remove_clause_from_formula(uint32_t index_to_remove, formula &f);
	static void unit_propagate(literal l, formula& f);

    static void check_sat_status(formula& f, assignment& partial);

	static literal choose_next_literal(formula& f, assignment& partial);

public:
	static void check_sat(formula& f);
};

#endif /* SATSOLVER_H_ */
