//
//  Benchmark.h
//  cuda-sat-solver
//
//  Created by Quentin Fiard on 18/03/13.
//  Copyright (c) 2013 École polytechnique. All rights reserved.
//

#ifndef __cuda_sat_solver__Benchmark__
#define __cuda_sat_solver__Benchmark__

#include <iostream>
#include <vector>
#include "types.h"

class Benchmark
{
protected:
    std::vector<formula> formulae;
    time_map t;
    double startTime;
    double endTime;
    bool* expected_sat;

    void setExpectedSat(bool* expected);
    bool hasExpectedSat();
    bool isSatExpected();

    void benchmarkStart();
    void benchmarkEnd();
    benchmark_data computeBenchmarkData();
    
    Benchmark(Benchmark& benchmark);
    void process();
    virtual void check_sat(formula& f);
    benchmark_data run();

private:
    void processBenchmarks();

public:
    Benchmark(std::vector<std::string> files, bool* is_sat = NULL);
    ~Benchmark();
    virtual std::string getName();
    
    static void startBenchmarks(Benchmark& main);
};

#endif /* defined(__cuda_sat_solver__Benchmark__) */
