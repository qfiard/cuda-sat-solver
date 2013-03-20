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
public:
	static void check_sat_given_partial_assignment(formula& f, assignment& partial, int level);

	static void process_unit_clauses(formula& f, assignment& partial);
	static void assign_pure_literals(formula& f, assignment& partial);

	static void remove_literal_from_clause(uint32_t index_to_remove, clause &c);
	static void remove_clause_from_formula(uint32_t index_to_remove, formula &f);
	static void unit_propagate(literal l, formula& f);

    static void check_sat_status(formula& f, assignment& partial);

	static literal choose_next_literal(formula& f, assignment& partial);

	static void check_sat(formula& f);
};

/*
 * For use in CUDA code
 */

literal choose_next_literal(formula& f, assignment& partial);
void remove_literal_from_clause(uint32_t index_to_remove, clause &c);
void remove_clause_from_formula(uint32_t index_to_remove, formula &f);

#endif /* SATSOLVER_H_ */
