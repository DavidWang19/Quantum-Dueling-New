#pragma once

#include "InitProblemInterface.h"

#include <vector>
#include <memory>

class Dueling
{
    public:
        ~Dueling() = default;
        Dueling() = default;
        Dueling(int N, int M, std::unique_ptr<InitProblemInterface> initProblemPtr);

        void setN(int N) { this->N = N; }
        void setM(int M) { this->M = M; }
        void setInitProblemPtr(std::unique_ptr<InitProblemInterface> initProblemPtr) { this->initProblemPtr = std::move(initProblemPtr); }
        std::unique_ptr<InitProblemInterface>& getInitProblemPtr() { return initProblemPtr; }
        int getClusterNum() { return clusterNum; }

        void initProblem();
        void recordParameters();
        void initStateVector();
        void G1();
        void G2();
        double getBestProb();
        std::vector<double> getResultsAll();
    protected:
        int N, M;
        std::unique_ptr<InitProblemInterface> initProblemPtr;

        int clusterNum;
        std::vector<long long> N_cnt;
        std::vector<int> sol;
        std::vector<bool> clusterIsSol;
        std::vector<std::vector<double>> phi, psi;
        std::vector<DataPoint> randomData;

        double sqr(double x) { return x * x; }
};