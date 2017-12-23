//
//  Benchmark.cpp
//  cuda-sat-solver
//
//  Created by Quentin Fiard on 18/03/13.
//  Copyright (c) 2013 École polytechnique. All rights reserved.
//

#include "Benchmark.h"
#include "CNFParser.h"
#include "UserInterface.h"
#include <dirent.h>
#include <sys/stat.h>
#include <algorithm>
#include <time.h>
#include <stdio.h>

#include "CPUBenchmark.h"
#include "CUDABenchmark.h"
#include "config.h"

static void add_files_with_parent(path parent, paths &v)
{
    FILE* file = fopen(parent.c_str(), "r");
    if(file != NULL)
    {
        fclose(file);

        struct stat statbuf;

        stat(parent.c_str(),&statbuf);

        if(S_ISDIR(statbuf.st_mode))
        {
            DIR* dir = opendir(parent.c_str());

            struct dirent *entry;
            while ((entry = readdir(dir)) != NULL)
            {
                std::string file_path(entry->d_name);
                if(file_path != "." && file_path != "..")
                {
                    file_path = parent+"/"+file_path;
                    add_files_with_parent(file_path, v);
                }
            }
            closedir(dir);
        }
        else
        {
            std::size_t n = parent.size();
            if(n>3 && parent.substr(n-3,3)=="cnf")
            {
                v.push_back(parent);
            }
        }
    }
}

Benchmark::Benchmark(paths files, bool* is_sat_expected)
{
    expected_sat = NULL;
    setExpectedSat(is_sat_expected);
    paths filtered_files;
    for(int i=0 ; i<files.size() ; i++)
    {
        add_files_with_parent(files[i], filtered_files);
    }
    if(filtered_files.size()==0)
    {
        throw "No file found in specified paths";
    }

    for(paths::iterator it = filtered_files.begin() ; it!=filtered_files.end() ; ++it)
    {
        try {
            formula f = CNFParser::parseFormulaFromFile(*it);
            this->formulae.push_back(f);
        }
        catch(...)
        {
            
        }
    }

    std::sort(this->formulae.begin(),this->formulae.end(),compare);

}

Benchmark::Benchmark(Benchmark& benchmark)
{
    expected_sat = NULL;
    setExpectedSat(benchmark.expected_sat);
    this->formulae.reserve(benchmark.formulae.size());
    for(int i=0 ; i<benchmark.formulae.size() ; i++)
    {
        this->formulae.push_back(deepcopy(benchmark.formulae[i]));
    }
}


Benchmark::~Benchmark()
{
    setExpectedSat(NULL);
}

benchmark_data Benchmark::run()
{
    this->benchmarkStart();
    this->process();
    this->benchmarkEnd();
    return this->computeBenchmarkData();
}

void Benchmark::check_sat(formula &f)
{
    throw "Should be overriden by subclass";
}

void Benchmark::processBenchmarks()
{
#ifdef RUN_CPU_BENCHMARK
    CPUBenchmark benchmark1(*this);
    benchmark_data d1 = benchmark1.run();

    UserInterface::print(d1);
#endif

#ifdef RUN_CUDA_BENCHMARK
    CUDABenchmark benchmark2(*this);
    benchmark_data d2 = benchmark2.run();

    UserInterface::print(d2);
#endif
}

void Benchmark::startBenchmarks(Benchmark &main)
{
    main.processBenchmarks();
}

std::string Benchmark::getName()
{
    return "Main benchmark";
}

void Benchmark::benchmarkStart()
{
    this->startTime = clock();
    this->startTime /= CLOCKS_PER_SEC;
}

void Benchmark::benchmarkEnd()
{
    this->endTime = clock();
    this->endTime /= CLOCKS_PER_SEC;
}

void Benchmark::setExpectedSat(bool* is_sat)
{
    if(is_sat != NULL && this->expected_sat == NULL)
    {
        this->expected_sat = new bool(is_sat);
    }
    if(is_sat == NULL && this->expected_sat != NULL)
    {
        delete this->expected_sat;
        this->expected_sat = NULL;
    }

    if(is_sat != NULL)
    {
        *(this->expected_sat) = *is_sat;
    }
}

bool Benchmark::hasExpectedSat()
{
    return (this->expected_sat != NULL);
}

bool Benchmark::isSatExpected()
{
    return *(this->expected_sat);
}

benchmark_data Benchmark::computeBenchmarkData()
{
    benchmark_data res;
    res.time_elapsed = this->endTime - this->startTime;
    res.time_for_function = this->t;
    res.name = this->getName();
    return res;
}

void Benchmark::process()
{
    for(int i=0 ; i<this->formulae.size() ; i++)
    {
        double progress = double(i+1)/this->formulae.size();

        formula& f = this->formulae[i];
        try {
        	this->check_sat(f);
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

#ifdef PRINT_ASSIGNMENT
            UserInterface::print(e.getAssigment());
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
