#include "include/Dueling.h"
#include "include/InitMethods.h"

#include <cmath>
#include <cstdio>
#include <algorithm>

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

int main()
{
    freopen("find_best_c.txt", "w", stdout);
    int N = 1 << 20;
    std::unique_ptr<InitProblemInterface> initProblem = std::make_unique<UniDistInit>();
    std::unique_ptr<Dueling> dueling = std::make_unique<Dueling>();
    dueling->setN(N);
    dueling->setInitProblemPtr(std::move(initProblem));
    printf("ALPHA=1..10, N=%d, M=1..SQRT(N)\n", N);
    for (int M = 1; M <= ceil(sqrt(N)); M++) {
        dueling->setM(M);
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
                for (int i = 0; i < alpha; i++) {
                    if (executeGate(dueling, 1, currentProb, Iter)) {
                        breakFlag = true;
                        break;
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
        printf("M=%d, bestAlpha=%d, Iter=%d, Prob=%lf\n", M, bestAlpha, bestIter, bestProb);
    }
    return 0;
}