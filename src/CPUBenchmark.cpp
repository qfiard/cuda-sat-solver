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

void CPUBenchmark::check_sat(formula& f)
{
	SATSolver::check_sat(f);
}

std::string CPUBenchmark::getName()
{
    return "CPU benchmark";
}
