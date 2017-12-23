# Makefile
# Created: Dec 23, 2017
# Author: roman3017
# Usage: PATH=/usr/local/cuda/bin:$PATH make
#        ./cuda-sat-solver sat test/sat1.cnf 

CUDA = /usr/local/cuda

INCLUDE = -I$(CUDA)/include
LIBS = -Wl,-rpath,$(CUDA)/lib64
LIBS += -L$(CUDA)/lib64 -lcuda -lcudart

CXX = g++
LD = g++

#CXXFLAGS = -O2 -Wall $(INCLUDE)
#LDFLAGS = -O2
CXXFLAGS = -g -Wall $(INCLUDE) -DVERBOSE_CNF_READ -DPRINT_FILE_NAMES -DPRINT_ASSIGNMENT -DRUN_CPU_BENCHMARK
LDFLAGS = -g

NVCC = $(CUDA)/bin/nvcc
NVCCFLAGS = -g -G -ccbin $(CXX) $(INCLUDE)

OBJ = src/main.o src/types.o src/SATSolver.o src/CUDASATSolver.o src/CNFParser.o src/Benchmark.o src/CUDABenchmark.o src/CPUBenchmark.o src/UserInterface.o
EXE = cuda-sat-solver

all: $(EXE)

.SUFFIXES: .cu

.cu.o:
	$(NVCC) $(NVCCFLAGS) -c $< -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXE): $(OBJ)
	$(LD) $(LDFLAGS) $(OBJ) $(LIBS) -o $@

clean:
	rm -f $(OBJ) $(EXE)
