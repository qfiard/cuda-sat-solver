/*
 * UserInterface.h
 *
 *  Created on: 17 mars 2013
 *      Author: quentinfiard
 */

#ifndef USERINTERFACE_H_
#define USERINTERFACE_H_

#include "types.h"

class UserInterface {
public:
	static void alert_unsat(UNSAT &e);
	static void alert_sat(SAT &e);
    static void printClause(clause& c);
    static void printFormula(formula& f);
    static void printAssignment(assignment& a);
};

#endif /* USERINTERFACE_H_ */
