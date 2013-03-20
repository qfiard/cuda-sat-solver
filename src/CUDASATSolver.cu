/* *
 * Copyright 1993-2012 NVIDIA Corporation.  All rights reserved.
 *
 * Please refer to the NVIDIA end user license agreement (EULA) associated
 * with this source code for terms and conditions that govern your use of
 * this software. Any use, reproduction, disclosure, or distribution of
 * this software and related documentation outside the terms of the EULA
 * is strictly prohibited.
 */
#include <stdio.h>
#include <stdlib.h>
#include "CUDASATSolver.h"
#include "config.h"

extern void remove_literal_from_clause(uint32_t index_to_remove, clause &c);
extern void remove_clause_from_formula(uint32_t index_to_remove, formula &f);
extern literal choose_next_literal(formula& f, assignment& partial);

assignment* check_sat_given_partial_assignment(formula& f, assignment& partial, int level); // DONE
void process_unit_clauses(formula& f, assignment& partial); // DONE
assignment* assign_pure_literals(formula& f, assignment& partial); // DONE
void unit_propagate(literal l, formula& f); // DONE
assignment* check_sat_status(formula& f, assignment& partial); // DONE

static const int WORK_SIZE = 256;

/**
 * This macro checks return value of the CUDA runtime call and exits
 * the application if the call failed.
 */
#define CUDA_CHECK_RETURN(value) {											\
	cudaError_t _m_cudaStat = value;										\
	if (_m_cudaStat != cudaSuccess) {										\
		fprintf(stderr, "Error %s at line %d in file %s\n",					\
				cudaGetErrorString(_m_cudaStat), __LINE__, __FILE__);		\
		exit(1);															\
	} }

__host__ __device__ unsigned int bitreverse(unsigned int number) {
	number = ((0xf0f0f0f0 & number) >> 4) | ((0x0f0f0f0f & number) << 4);
	number = ((0xcccccccc & number) >> 2) | ((0x33333333 & number) << 2);
	number = ((0xaaaaaaaa & number) >> 1) | ((0x55555555 & number) << 1);
	return number;
}

/**
 * CUDA kernel function that reverses the order of bits in each element of the array.
 */
__global__ void bitreverse(void *data) {
	unsigned int *idata = (unsigned int*) data;
	idata[threadIdx.x] = bitreverse(idata[threadIdx.x]);
}

/**
 * Host function that prepares data array and passes it to the CUDA kernel.
 */

void unit_propagate(literal l, formula& f)
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

void process_unit_clauses(formula& f, assignment& partial)
{
	for(int i=0 ; i<f.nbOfClauses ; i++)
	{
		clause& c = f.clauses[i];
		if(c.length == 1)
		{
			literal& l = c.literals[0];

            if(partial.literals[abs(l)]==0)
            {
                if(l>0)
                {
                    partial.literals[l] = 1;
                }
                else
                {
                    partial.literals[-l] = -1;
                }
                unit_propagate(l,f);

                process_unit_clauses(f,partial);
            }
		}
	}
}

assignment* assign_pure_literals(formula& f, assignment& partial)
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
		if(partial.literals[i] == 0 && positive[i] ^ negative[i])
		{
			if(positive[i])
			{
				partial.literals[i] = 1;
				unit_propagate(i,f);
				changed = true;
			}
			else
			{
				partial.literals[i] = -1;
				unit_propagate(-i,f);
				changed = true;
			}

		}
	}

    delete[] positive;
    delete[] negative;

	if(changed)
	{
        assignment* res = check_sat_status(f,partial);
        if(res!=NULL)
        {
        	return res;
        }

		process_unit_clauses(f,partial);
		assign_pure_literals(f,partial);
	}

	return NULL;
}

assignment* check_sat_status(formula& f, assignment& partial)
{
    if(f.nbOfClauses==0)
	{
    	assignment* res = new assignment;
    	*res = deepcopy(partial);
    	res->unsat = false;
    	return res;
	}

	for(int i=0 ; i<f.nbOfClauses ; i++)
	{
		clause& c = f.clauses[i];
		if(c.length == 0)
		{
			assignment* res = new assignment;
			res->unsat = true;
			return res;
		}
	}
	return NULL;
}

#ifdef PRINT_BACKTRACKING
static int min_backtrack_level = -1;
#endif

assignment* check_sat_given_partial_assignment(formula& f, assignment& partial, int level)
{
	assignment* res;

	res = check_sat_status(f,partial);
	if(res!=NULL)
	{
		return res;
	}

	process_unit_clauses(f,partial);

	res = assign_pure_literals(f,partial);
	if(res!=NULL)
	{
		return res;
	}

	res = check_sat_status(f,partial);
	if(res!=NULL)
	{
		return res;
	}

    formula fbis;

    literal l = choose_next_literal(f, partial);
    if(l==0)
    {
    	assignment* res = new assignment;
		res->unsat = true;
		return res;
    }

    if(l>0)
    {
    	assignment a = deepcopy(partial);
		a.literals[l] = 1;
		fbis = copy(f);
		unit_propagate(l,fbis);
		res = check_sat_given_partial_assignment(fbis,a,level+1);
		if(!res->unsat)
		{
			return res;
		}

		// Left subtree was UNSAT

#ifdef PRINT_BACKTRACKING
		if(min_backtrack_level == -1 || min_backtrack_level>level)
		{
			min_backtrack_level = level;
			printf("Backtracking to level %d\n",level);
		}
#endif
		partial.literals[l] = -1;
		dealloc(fbis);
		unit_propagate(-l,f);
		return check_sat_given_partial_assignment(f,partial,level+1);
    }
    else
    {
    	assignment a = deepcopy(partial);
		a.literals[l] = -1;
		fbis = copy(f);
		unit_propagate(l,fbis);
		res = check_sat_given_partial_assignment(fbis,a,level+1);

		if(!res->unsat)
		{
			return res;
		}

		// Left subtree was UNSAT

#ifdef PRINT_BACKTRACKING
		if(min_backtrack_level == -1 || min_backtrack_level>level)
		{
			min_backtrack_level = level;
			printf("Backtracking to level %d\n",level);
		}
#endif
		partial.literals[l] = 1;
		dealloc(fbis);
		unit_propagate(-l,f);
		return check_sat_given_partial_assignment(f,partial,level+1);
    }
}

assignment* cuda_check_sat(formula& f) {
	assignment partial;
	partial.length = f.nbOfVariables;
	partial.literals = new literal[partial.length];
	for(int i=0 ; i<partial.length ; i++)
	{
		partial.literals[i] = 0;
	}
	return check_sat_given_partial_assignment(f,partial,0);
}
