#include "InitApp.h"

#include <cmath>
#include <random>

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

// void OPNLInit::initProblem(std::vector<DataPoint>& randomData, int N, int M)
// {
//     randomData.resize(N);
//     int K = ceil(double(N) / M);
//     for (int i = 0; i < N; i++) {
//         randomData[i].value = i;
//         randomData[i].isSolution = false;
//     }
//     for (int i = startIndex; i < N; i += K) {
//         randomData[i].isSolution = true;
//     }
// }