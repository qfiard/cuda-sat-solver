//
//  CPUBenchmark.h
//  cuda-sat-solver
//
//  Created by Quentin Fiard on 18/03/13.
//  Copyright (c) 2013 École polytechnique. All rights reserved.
//

#ifndef __cuda_sat_solver__CPUBenchmark__
#define __cuda_sat_solver__CPUBenchmark__

#include "Benchmark.h"
#include "types.h"

class CPUBenchmark : public Benchmark
{
protected:
    CPUBenchmark(Benchmark& benchmark);
    void check_sat(formula& f);
public:
    std::string getName();

    friend class Benchmark;
};

#endif /* defined(__cuda_sat_solver__CPUBenchmark__) */
