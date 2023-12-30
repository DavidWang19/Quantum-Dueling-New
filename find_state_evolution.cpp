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

constexpr bool SINGLE_GATE_ALPHA = false;
constexpr int BEST_ALPHA_FOUND = 1;
constexpr int ITER_THRESHOLD = 1000;

int main()
{
    freopen("find_state_evolution.txt", "w", stdout);
    int N = 1 << 12;
    int M = 1 << 6;
    std::unique_ptr<InitProblemInterface> initProblem = std::make_unique<UniDistInit>();
    std::unique_ptr<Dueling> dueling = std::make_unique<Dueling>(N, M, std::move(initProblem));
    //static_cast<UniDistInit*>(dueling->getInitProblemPtr().get())->setStartIndex(N / M / 2);
    dueling->initProblem();
    dueling->recordParameters();
    int bestAlpha = 0, bestIter = 0;
    double bestProb = 0;
    std::vector<double> bestProbVec;
    if (BEST_ALPHA_FOUND) {
        dueling->initStateVector();
        double currentProb = dueling->getBestProb();
        bestProbVec.emplace_back(currentProb);
        int Iter = 0;
        while (true) {
            if (!SINGLE_GATE_ALPHA) {
                for (int i = 0; i < BEST_ALPHA_FOUND; i++) {
                    dueling->G1();
                    currentProb = dueling->getBestProb();
                    bestProbVec.emplace_back(currentProb);
                    Iter++;
                    if (Iter >= ITER_THRESHOLD) break;
                }
            } else {
                dueling->G1();
                currentProb = dueling->getBestProb();
                bestProbVec.emplace_back(currentProb);
                Iter++;
                if (Iter >= ITER_THRESHOLD) break;
            }
            if (Iter >= ITER_THRESHOLD) break;
            for (int i = 0; i < BEST_ALPHA_FOUND; i++) {
                dueling->G2();
                currentProb = dueling->getBestProb();
                bestProbVec.emplace_back(currentProb);
                Iter++;
                if (Iter >= ITER_THRESHOLD) break;
            }
            if (Iter >= ITER_THRESHOLD) break;
        }
    } else {
        for (int alpha = 1;; alpha++) {
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
            if (Iter < alpha) break;
        }
    }
    if (!BEST_ALPHA_FOUND) {
        std::cerr << "N=" << N << ", M=" << M << ", bestAlpha=" << bestAlpha << ", Iter=" << bestIter << ", Prob=" << bestProb << std::endl;
    } else {
        for (const auto& prob : bestProbVec) {
            printf("%.8lf\n", prob);
        }
    }
    return 0;
}