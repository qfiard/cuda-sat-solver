#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <vector>
#include <time.h>

#include "Benchmark.h"

#include <iostream>
#include <typeinfo>

int main(int argc, char *argv[]) {
    if(argc < 2)
    {
        printf("Usage : %s sat file_or_folder1 file_or_folder2 ...",argv[0]);
        return 0;
    }

    try {
        bool* sat_expected = NULL;
        std::vector<std::string> files;
        for(int i=1 ; i<argc ; i++)
        {
            std::string arg = argv[i];
            if(arg=="sat")
            {
                if(sat_expected==NULL)
                {
                    sat_expected = new bool;
                }
                *sat_expected = true;
            }
            else if(arg=="unsat")
            {
                if(sat_expected==NULL)
                {
                    sat_expected = new bool;
                }
                *sat_expected = false;
            }
            else
            {
                files.push_back(argv[i]);
            }
        }

        Benchmark benchmark(files,sat_expected);
        if(sat_expected != NULL)
        {
            delete sat_expected;
            sat_expected = NULL;
        }
        Benchmark::startBenchmarks(benchmark);
    }
    catch(std::string s)
    {
        printf("%s",s.c_str());
        throw;
    }
}

