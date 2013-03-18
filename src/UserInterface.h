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
    static void print(clause& c);
    static void print(formula& f);
    static void print(assignment& a);
    static void print(benchmark_data& d);
};

#endif /* USERINTERFACE_H_ */
