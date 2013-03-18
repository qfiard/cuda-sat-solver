//
//  CUDABenchmark.cpp
//  cuda-sat-solver
//
//  Created by Quentin Fiard on 18/03/13.
//  Copyright (c) 2013 École polytechnique. All rights reserved.
//

#include "CUDABenchmark.h"

CUDABenchmark::CUDABenchmark(Benchmark& benchmark) : Benchmark(benchmark)
{

}

void CUDABenchmark::process()
{
    
}

std::string CUDABenchmark::getName()
{
    return "CUDA Benchmark";
}