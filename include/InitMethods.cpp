#include "InitMethods.h"

#include <cmath>
#include <random>

void UniDistInit::initProblem(std::vector<DataPoint>& randomData, int N, int M)
{
    randomData.resize(N);
    int K = ceil(double(N) / M);
    for (int i = 0; i < N; i++) {
        randomData[i].value = i;
        randomData[i].isSolution = false;
    }
    for (int i = startIndex; i < N; i += K) {
        randomData[i].isSolution = true;
    }
}

void RandomInit::initProblem(std::vector<DataPoint>& randomData, int N, int M)
{
    randomData.resize(N);
    for (int i = 0; i < N; i++) {
        randomData[i].value = i;
        randomData[i].isSolution = false;
    }
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, N - 1);
    int M_cnt = M;
    while (M_cnt > 0) {
        int pos = dist(mt);
        if (randomData[pos].isSolution) continue;
        randomData[pos].isSolution = true;
        M_cnt--;
    }
}