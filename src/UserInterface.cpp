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

void UserInterface::print(assignment& a)
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
    print(a);
}

void UserInterface::print(clause& c)
{
    for(int i=0 ; i<c.length ; i++)
    {
        printf("%d ",c.literals[i]);
    }
    printf("\n");
}

void UserInterface::print(formula& f)
{
    printf("\n********************************\n");
    printf("Formula\n");
    printf("Source file : %s\n",f.source_file.c_str());
    for(int i=0 ; i<f.nbOfClauses ; i++)
    {
        print(f.clauses[i]);
    }
    printf("END Formula\n");
    printf("********************************\n\n");
}

void UserInterface::print(benchmark_data &d)
{
    printf("Benchmark result\n");
    printf("Benchmark name : %s\n", d.name.c_str());
    printf("Time elapsed : %fs\n", d.time_elapsed);
    for(std::map<std::string, double>::iterator i = d.time_for_function.begin() ; i!=d.time_for_function.end() ; ++i)
    {
        printf("Time elapsed in function %s : %fs (%f%%)\n", i->first.c_str(), i->second, i->second/d.time_elapsed);
    }
    printf("\n");
}