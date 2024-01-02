#include "InitApp.h"

#include <cmath>
#include <random>
#include <iostream>

/*
 * Optimization Problem without Solution Labeling
 * Solution to Non-solution Ratio : 1:0, 1:1, 1:3, 1:7 .. 
 * Each solution has distinct values, each non-solution's value = its corresponding solution's value + 1/2 (ie. 1 for IP)
 * For each M -> IV: M/N ratio  DV: numIter/Prob 
 * 2:2 N = 4 M = 2, 4:4 N = 8 M = 4, 8:8 N = 16 M = 8...
 * M -> ratio -> bin search a 
 * alpha = beta = c
 * 1 - (1 - pmax)^(a+1) >= pbar >= 1 - (1 - pmax)^a
 * pbar = 1 - (1 - pmax)^a * (1 - p0)
 * p0 = 1 - (1 - pbar) / (1 - pmax)^a
 * pbar = 2/3, 3/4
 * 
 */
void OPNLInit::initProblem(std::vector<DataPoint>& randomData, int N, int M)
{
    randomData.resize(N);
    for (int i = 0; i < N; i ++) {
        randomData[i].value = i % M * 2;
        randomData[i].isSolution = true;
    }

    for (int i = M; i < N; i ++) {
        randomData[i].value += 1;
        randomData[i].isSolution = false;
    }

}

void PSATInit::initProblem(std::vector<DataPoint>& randomData, int N, int M)
{
    int actual_M = 0;
    int numBestSol, bestSolVal;
    randomData.resize(N);
    while (actual_M == 0) {
        PSAT PSAT_inst(M, N);
        actual_M = 0;
        numBestSol = 0;
        bestSolVal = 0;
        for (int i = 0; i < N; i++) {
            randomData[i].value = -1 * (std::get<0> (PSAT_inst.assignments[i]));
            randomData[i].isSolution = (std::get<1> (PSAT_inst.assignments[i]));
            if (randomData[i].isSolution) {
                actual_M ++;
            }
            // } else {
            //      randomData[i].value += static_cast<int> ((PSAT_inst.numClause) / 4);
            // }
            if (bestSolVal == 0 ||  randomData[i].value > bestSolVal) {
                numBestSol = 1;
            } else if (randomData[i].value == bestSolVal) {
                numBestSol += 1;
            }
        }
    }

    std::cout << "Actual M = " << actual_M << std::endl;
    std::cout << "Number of Best Solutions = " << numBestSol << std::endl;
    
}
