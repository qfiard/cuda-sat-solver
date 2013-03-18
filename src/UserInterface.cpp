/*
 * UserInterface.cpp
 *
 *  Created on: 17 mars 2013
 *      Author: quentinfiard
 */

#include "UserInterface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>

void UserInterface::alert_unsat(UNSAT &e)
{
	printf("UNSAT !\n");
}

void UserInterface::printAssignment(assignment& a)
{
	for(int i=1 ; i<a.size() ; i++)
	{
		std::string disp_value = "true";
		if(a[i]==0)
		{
			disp_value = "not important";
		}
		if(a[i]==-1)
		{
			disp_value = "false";
		}
		printf("\tVar %d -> %s\n", i, disp_value.c_str());
	}
    printf("\n");
}

void UserInterface::alert_sat(SAT &e)
{
	printf("SAT !\n");
	printf("Satisfied by assignment :\n");
	assignment& a = e.getAssigment();
    printAssignment(a);
}

void UserInterface::printClause(clause& c)
{
    for(int i=0 ; i<c.length ; i++)
    {
        printf("%d ",c.literals[i]);
    }
    printf("\n");
}

void UserInterface::printFormula(formula& f)
{
    printf("\n********************************\n");
    printf("Formula\n");
    for(int i=0 ; i<f.nbOfClauses ; i++)
    {
        printClause(f.clauses[i]);
    }
    printf("END Formula\n");
    printf("********************************\n\n");
}
