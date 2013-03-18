#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include "types.h"
#include "CNFParser.h"
#include "SATSolver.h"
#include "UserInterface.h"
#include <time.h>

#include <iostream>
#include <typeinfo>

char host_solution[501];

int main(int argc, char *argv[]) {

    try {
	std::string file = "/Users/quentinfiard/dev/cuda-sat-solver/test/to_test.cnf";

	formula f = CNFParser::parseFormulaFromFile(file);

	printf("Starting SAT resolution on CPU...\n");

	double start = (double)clock()/CLOCKS_PER_SEC;

	try {
		SATSolver::check_sat(f);
	}
	catch(UNSAT &e)
	{
		UserInterface::alert_unsat(e);
	}
	catch(SAT &e)
	{
		UserInterface::alert_sat(e);
	}



	double end = (double)clock()/CLOCKS_PER_SEC;

	printf("Time elapsed : %f seconds\n\n", end-start);

	printf("Test");
	return 0;
    }
    catch(std::string s)
    {
        printf("%s",s.c_str());
        throw;
    }
}

