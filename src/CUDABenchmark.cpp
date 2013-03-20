//
//  CUDABenchmark.cpp
//  cuda-sat-solver
//
//  Created by Quentin Fiard on 18/03/13.
//  Copyright (c) 2013 École polytechnique. All rights reserved.
//

#include "CUDABenchmark.h"
#include "CUDASATSolver.h"

CUDABenchmark::CUDABenchmark(Benchmark& benchmark) : Benchmark(benchmark)
{

}

void CUDABenchmark::check_sat(formula& f)
{
	assignment* a = cuda_check_sat(f);
	if(a->unsat)
	{
		dealloc(*a);
		delete a;

		throw UNSAT();
	}
	else
	{
		SAT e(*a);
		deep_dealloc(*a);
		delete a;

		throw e;
	}
}

std::string CUDABenchmark::getName()
{
    return "CUDA Benchmark";
}
