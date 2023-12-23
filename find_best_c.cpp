#include "include/Dueling.h"
#include "include/InitMethods.h"

#include <cmath>
#include <cstdio>
#include <algorithm>
#include <iostream>

bool executeGate(std::unique_ptr<Dueling>& dueling, int target, double& currentProb, int& Iter) {
    if (target == 1) {
        dueling->G1();
    } else {
        dueling->G2();
    }
    double prob = dueling->getBestProb();
    if (prob > currentProb) {
        currentProb = prob;
        Iter++;
        return false;
    } else {
        return true;
    }
}

constexpr bool SINGLE_GATE_ALPHA = true;

int main()
{
    //freopen("find_best_c.txt", "w", stdout);
    int N = 1 << 12;
    std::unique_ptr<InitProblemInterface> initProblem = std::make_unique<RandomInit>();
    std::unique_ptr<Dueling> dueling = std::make_unique<Dueling>();
    dueling->setN(N);
    dueling->setInitProblemPtr(std::move(initProblem));
    for (int lgM = 0; lgM <= 11; lgM++) {
        int M = 1 << lgM;
        dueling->setM(M);
        //static_cast<UniDistInit*>(dueling->getInitProblemPtr().get())->setStartIndex(N / M / 2);
        dueling->initProblem();
        dueling->recordParameters();
        int bestAlpha = 0, bestIter = 0;
        double bestProb = 0;
        for (int alpha = 1; alpha <= 10; alpha++) {
            dueling->initStateVector();
            bool breakFlag = false;
            int Iter = 0;
            double currentProb = dueling->getBestProb();
            while (true) {
                if (!SINGLE_GATE_ALPHA) {
                    for (int i = 0; i < alpha; i++) {
                        if (executeGate(dueling, 1, currentProb, Iter)) {
                            breakFlag = true;
                            break;
                        }
                    }
                } else {
                    if (executeGate(dueling, 1, currentProb, Iter)) {
                        breakFlag = true;
                    }
                }
                if (breakFlag) break;
                for (int i = 0; i < alpha; i++) {
                    if (executeGate(dueling, 2, currentProb, Iter)) {
                        breakFlag = true;
                        break;
                    }
                }
                if (breakFlag) break;
            }
            if (currentProb > bestProb) {
                bestProb = currentProb;
                bestAlpha = alpha;
                bestIter = Iter;
            }
        }
        //printf("N=%d, M=%d, bestAlpha=%d, Iter=%d, Prob=%lf\n", N, M, bestAlpha, bestIter, bestProb);
        std::cerr << "N=" << N << ", M=" << M << ", bestAlpha=" << bestAlpha << ", Iter=" << bestIter << ", Prob=" << bestProb << std::endl;
    }
    return 0;
}