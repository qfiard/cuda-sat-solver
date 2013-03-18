//
//  CPUBenchmark.cpp
//  cuda-sat-solver
//
//  Created by Quentin Fiard on 18/03/13.
//  Copyright (c) 2013 École polytechnique. All rights reserved.
//

#include "CPUBenchmark.h"
#include "SATSolver.h"
#include "math.h"

CPUBenchmark::CPUBenchmark(Benchmark& benchmark) : Benchmark(benchmark)
{
    
}

void CPUBenchmark::process()
{
    for(int i=0 ; i<this->formulae.size() ; i++)
    {
        double progress = double(i+1)/this->formulae.size();
        
        formula& f = this->formulae[i];
        try {
            SATSolver::check_sat(f);
        }
        catch(SAT &e)
        {
            printf("%5.1f%% ",100*progress);
            if(this->hasExpectedSat())
            {
                if(this->isSatExpected())
                {
                    printf("[SUCCESS] ");
                }
                else
                {
                    printf("[FAILED] ");
                }
            }
#ifdef PRINT_FILE_NAMES
            printf("SAT %s\n", f.source_file.c_str());
#else
            printf("SAT\n");
#endif
        }
        catch(UNSAT &e)
        {
            printf("%5.1f%% ",100*progress);
            if(this->hasExpectedSat())
            {
                if(!this->isSatExpected())
                {
                    printf("[SUCCESS] ");
                }
                else
                {
                    printf("[FAILED] ");
                }
            }
#ifdef PRINT_FILE_NAMES
            printf("UNSAT %s\n", f.source_file.c_str());
#else
            printf("UNSAT\n");
#endif
        }
    }
}

std::string CPUBenchmark::getName()
{
    return "CPU benchmark";
}