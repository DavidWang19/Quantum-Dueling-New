#include "include/Dueling.h"
#include "include/InitApp.h"

#include <cmath>
#include <cstdio>
#include <algorithm>
#include <iostream>
#include <vector>

#define P_BAR (2.0/3.0)

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

/*
 * 1 - (1 - pmax)^(a+1) >= pbar >= 1 - (1 - pmax)^a
 * pbar = 2/3, 3/4
 * 
 * log_(1-pmax)(1-pbar) >= a >= log_(1-pmax)(1-pbar) - 1
 * a * iterPerRound
 * IV M DV  a * iterPerRound (ie. total number of iteration)
 * 
 */
int calcTotalIter(double pmax, int iterPerRound, const std::vector<int> &iters, const std::vector<double> &probs) {
    int a = static_cast<int> (floor(log(1 - P_BAR) / log(1 - pmax)));
    double p0 = 1 - ((1 - P_BAR) / (pow((1-pmax), a)));
    for (size_t i = 0; i < probs.size(); i ++) {
        if (probs[i] >= p0) {
            return iters[i] + a * iterPerRound;
        }
    }
    return (a + 1) * iterPerRound;;
}

/* Main function for OPNL */
int main() {
    //freopen("find_best_c.txt", "w", stdout);
    std::cout << "Initializing" << std::endl; // Display the input value
    std::vector<int> iters {};
    std::vector<double> probs {};
    std::vector<int> best_iters {};
    std::vector<double> best_probs {};
    for (int M_pow = 0; M_pow <= 10; M_pow++) {
        int M = 1 << M_pow;
        std::cout << "M = " << M << std::endl; // Display the input value
        int bestTotalIter = INT_MAX;
        int bestRatio = 0;
        // increase delta_n qubits
        for (int ratio_pow = 1; ; ratio_pow += 1) {
            int ratio = 1 << ratio_pow; // eta
            int N = M * ratio;
            std::unique_ptr<InitProblemInterface> initProblem = std::make_unique<OPNLInit>();
            std::unique_ptr<Dueling> dueling = std::make_unique<Dueling>();
            dueling->setN(N);
            dueling->setInitProblemPtr(std::move(initProblem));
            dueling->setM(M);
            //static_cast<UniDistInit*>(dueling->getInitProblemPtr().get())->setStartIndex(N / M / 2);
            dueling->initProblem();
            dueling->recordParameters();
            int bestAlpha = 0, bestIter = 0;
            double bestProb = 0;

            for (int alpha = 1; alpha <= 15; alpha++) {
                iters.clear();
                probs.clear();
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
                        iters.push_back(Iter);
                        probs.push_back(currentProb);
                    }
                    if (breakFlag) break;
                    for (int i = 0; i < alpha; i++) {
                        if (executeGate(dueling, 2, currentProb, Iter)) {
                            breakFlag = true;
                            break;
                        }
                        iters.push_back(Iter);
                        probs.push_back(currentProb);
                    }
                    if (breakFlag) break;
                }

                if (currentProb > bestProb) {
                    bestProb = currentProb;
                    bestAlpha = alpha;
                    bestIter = Iter;
                    best_iters = iters;
                    best_probs = probs;
                }
            }
            //printf("N=%d, M=%d, bestAlpha=%d, Iter=%d, Prob=%lf\n", N, M, bestAlpha, bestIter, bestProb);
            std::cerr << "ratio= 1/" << ratio << ", N=" << N << ", bestAlpha=" << bestAlpha << ", Iter=" << bestIter << ", Prob=" << bestProb << std::endl;
            int total_iter = calcTotalIter(bestProb, bestIter, best_iters, best_probs);
            // not searching for more ratio if total dueling rounds not increase anymore
            if (total_iter <  bestTotalIter) {
                bestTotalIter = total_iter;
                bestRatio = ratio;
            } else {
                break;
            }

        }
        std::cout << "BestTotalIter is " << bestTotalIter << std::endl;
        std::cout << "Best M to N ratio is 1/" << bestRatio << std::endl; 
    }
    return 0;
}