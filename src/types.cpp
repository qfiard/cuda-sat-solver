//
//  types.cpp
//  cuda-sat-solver
//
//  Created by Quentin Fiard on 18/03/13.
//  Copyright (c) 2013 École polytechnique. All rights reserved.
//

#include "types.h"
#include <math.h>

static double complexity(formula f)
{
    return sqrt(f.nbOfClauses*f.nbOfClauses + f.nbOfClauses*f.nbOfClauses);
}

bool compare(formula f1, formula f2)
{
    return complexity(f1)<complexity(f2);
}

clause copy(clause& c)
{
    clause res = c;
    return res;
}

assignment copy(assignment& a)
{
	assignment res = a;
    return res;
}

formula copy(formula& f)
{
    formula res = f;

    res.initialNbOfClauses = f.nbOfClauses;
    res.clauses = new clause[f.nbOfClauses];

    for(int i=0 ; i<f.nbOfClauses ; i++)
    {
        res.clauses[i] = copy(f.clauses[i]);
    }

    return res;
}

clause deepcopy(clause& c)
{
    clause res = c;

    res.literals = new literal[c.length];

    for(int i=0 ; i<c.length ; i++)
    {
        res.literals[i] = c.literals[i];
    }

    return res;
}

assignment deepcopy(assignment& a)
{
	assignment res = a;

    res.literals = new literal[a.length];

    for(int i=0 ; i<a.length ; i++)
    {
        res.literals[i] = a.literals[i];
    }

    return res;
}

formula deepcopy(formula& f)
{
    formula res = f;

    res.initialNbOfClauses = f.nbOfClauses;
    res.clauses = new clause[f.nbOfClauses];

    for(int i=0 ; i<f.nbOfClauses ; i++)
    {
        res.clauses[i] = deepcopy(f.clauses[i]);
    }

    return res;
}

void dealloc(clause& c)
{
    
}

void dealloc(formula& f)
{
    for(int i=0 ; i<f.initialNbOfClauses ; i++)
    {
        dealloc(f.clauses[i]);
    }

    delete[] f.clauses;
}

void deep_dealloc(clause& c)
{
    delete[] c.literals;
}

void deep_dealloc(formula& f)
{
    for(int i=0 ; i<f.initialNbOfClauses ; i++)
    {
        deep_dealloc(f.clauses[i]);
    }

    delete[] f.clauses;
}
