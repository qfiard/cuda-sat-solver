//
//  CudaBenchmark.h
//  cuda-sat-solver
//
//  Created by Quentin Fiard on 18/03/13.
//  Copyright (c) 2013 École polytechnique. All rights reserved.
//

#ifndef __cuda_sat_solver__CUDABenchmark__
#define __cuda_sat_solver__CUDABenchmark__

#include "Benchmark.h"
#include "types.h"

class CUDABenchmark : public Benchmark
{
protected:
    CUDABenchmark(Benchmark& benchmark);
    void process();
public:
    std::string getName();

    friend class Benchmark;
};

#endif /* defined(__cuda_sat_solver__CudaBenchmark__) */
