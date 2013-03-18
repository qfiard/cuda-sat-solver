

/*
 * SATSolver.cpp
 *
 *  Created on: 17 mars 2013
 *      Author: quentinfiard
 */

#include "SATSolver.h"
#include <math.h>
#include "UserInterface.h"

inline static int abs(int a)
{
    if(a>0)
    {
        return a;
    }
    return -a;
}

void SATSolver::remove_literal_from_clause(uint32_t index_to_remove, clause &c)
{
	//We exchange this literal and the last one in c
	literal tmp = c.literals[c.length-1];
	c.literals[c.length-1] = c.literals[index_to_remove];
	c.literals[index_to_remove] = tmp;

	c.length--;
}

void SATSolver::remove_clause_from_formula(uint32_t index_to_remove, formula &f)
{
	//We exchange this clause and the last one in f
	clause tmp = f.clauses[f.nbOfClauses-1];
	f.clauses[f.nbOfClauses-1] = f.clauses[index_to_remove];
	f.clauses[index_to_remove] = tmp;

	f.nbOfClauses--;
}

void SATSolver::unit_propagate(literal l, formula& f)
{
	for(int i=0 ; i<f.nbOfClauses ; i++)
	{
		clause& c = f.clauses[i];
		for(int j=0 ; j<c.length ; j++)
		{
			if(c.literals[j]==l)
			{
				remove_clause_from_formula(i,f);
				i--;
				break;
			}
			if(c.literals[j]==-l)
			{
				remove_literal_from_clause(j,c);
				j--;
			}
		}
	}
}

void SATSolver::process_unit_clauses(formula& f, assignment& partial)
{
	for(int i=0 ; i<f.nbOfClauses ; i++)
	{
		clause& c = f.clauses[i];
		if(c.length == 1)
		{
			literal& l = c.literals[0];

            if(partial[abs(l)]==0)
            {
                if(l>0)
                {
                    partial[l] = 1;
                }
                else
                {
                    partial[-l] = -1;
                }
                unit_propagate(l,f);

                process_unit_clauses(f,partial);
            }
		}
	}
}

void SATSolver::assign_pure_literals(formula& f, assignment& partial)
{
	bool* positive = new bool[f.nbOfVariables+1];
	bool* negative = new bool[f.nbOfVariables+1];

	for(int i=0 ; i<f.nbOfVariables+1 ; i++)
	{
		positive[i] = negative[i] = false;
	}

	for(int i=0 ; i<f.nbOfClauses ; i++)
	{
		clause& c = f.clauses[i];
		for(int j=0 ; j<c.length ; j++)
		{
			literal& l = c.literals[j];
			if(l>0)
			{
				positive[l] = true;
			}
			else
			{
				negative[-l] = true;
			}
		}
	}

	bool changed = false;

	for(int i=1 ; i<f.nbOfVariables+1 ; i++)
	{
		if(partial[i] == 0 && positive[i] ^ negative[i])
		{
			if(positive[i])
			{
				partial[i] = 1;
				unit_propagate(i,f);
				changed = true;
			}
			else
			{
				partial[i] = -1;
				unit_propagate(-i,f);
				changed = true;
			}

		}
	}

    delete[] positive;
    delete[] negative;

	if(changed)
	{
        check_sat_status(f,partial);

		process_unit_clauses(f,partial);
		assign_pure_literals(f,partial);
	}
}

literal SATSolver::choose_next_literal(formula& f, assignment& partial)
{
	for(int i=0 ; i<f.nbOfClauses ; i++)
	{
		clause& c = f.clauses[i];
		for(int j=0 ; j<c.length ; j++)
		{
			literal& l = c.literals[j];
			if(partial[abs(l)] == 0)
			{
				return l;
			}
		}
	}
    throw UNSAT();
}

void SATSolver::check_sat_status(formula& f, assignment& partial)
{
    if(f.nbOfClauses==0)
	{
		throw SAT(partial);
	}

	for(int i=0 ; i<f.nbOfClauses ; i++)
	{
		clause& c = f.clauses[i];
		if(c.length == 0)
		{
			throw UNSAT();
		}
	}

    bool test = false;
    for(int i = 1 ; i<partial.size() ; i++)
    {
        test |= (partial[i]==0);
    }
    if(!test)
    {
        printf("Test");
    }
}

#ifdef PRINT_BACKTRACKING
int min_backtrack_level = -1;
#endif

void SATSolver::check_sat_given_partial_assignment(formula& f, assignment& partial, int level)
{
	check_sat_status(f,partial);

	process_unit_clauses(f,partial);
	assign_pure_literals(f,partial);

	check_sat_status(f,partial);

    formula fbis;

    literal l = choose_next_literal(f, partial);

    if(l>0)
    {
        try{
            assignment a = deepcopy(partial);
            a[l] = 1;
            fbis = copy(f);
            unit_propagate(l,fbis);
    	    check_sat_given_partial_assignment(fbis,a,level+1);
        }
        catch(UNSAT &e)
        {
#ifdef PRINT_BACKTRACKING
            if(min_backtrack_level == -1 || min_backtrack_level>level)
            {
                min_backtrack_level = level;
                printf("Backtracking to level %d\n",level);
            }
#endif
            partial[l] = -1;
            dealloc(fbis);
            unit_propagate(-l,f);
    	    check_sat_given_partial_assignment(f,partial,level+1);
        }
    }
    else
    {
        try{
            assignment a = deepcopy(partial);
            a[-l] = -1;
            fbis = copy(f);
            unit_propagate(l,fbis);
    	    check_sat_given_partial_assignment(fbis,a,level+1);
        }
        catch(UNSAT &e)
        {
#ifdef PRINT_BACKTRACKING
            if(min_backtrack_level == -1 || min_backtrack_level>level)
            {
                min_backtrack_level = level;
                printf("Backtracking to level %d\n",level);
            }
#endif
            partial[-l] = 1;
            dealloc(fbis);
            unit_propagate(-l,f);
            check_sat_given_partial_assignment(f,partial,level+1);
        }
    }
    
}

void SATSolver::check_sat(formula& f)
{
	assignment* partial = new assignment(f.nbOfVariables+1);
    for(int i=0 ; i<partial->size() ; i++)
    {
        partial->at(i) = 0;
    }
	check_sat_given_partial_assignment(f, *partial, 0);
}
