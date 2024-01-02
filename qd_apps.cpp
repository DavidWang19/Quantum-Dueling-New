#include "include/Dueling.h"
#include "include/InitApp.h"

#include <cmath>
#include <cstdio>
#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdexcept>

#define P_BAR (2.0/3.0)
#define Problem 2 // 0 for OPNL, 1 for PSAT, 2 fpr detailed PSAT
#define P3SAT_REPEAT_NUM 1000
#define N_POW_BOUND 12

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
 * pbar = 2/3
 * 
 * log_(1-pmax)(1-pbar) >= a >= log_(1-pmax)(1-pbar) - 1
 * a * iterPerRound
 * IV M DV  a * iterPerRound (ie. total number of iteration)
 */
int calcTotalIter(double pmax, int iterPerRound, const std::vector<int> &iters, const std::vector<double> &probs) {
    if (std::abs(pmax - 1.0) < 1e-6) return iterPerRound;
    int a = static_cast<int> (floor(log(1 - P_BAR) / log(1 - pmax)));
    double p0 = 1 - ((1 - P_BAR) / (pow((1-pmax), a)));
    for (size_t i = 0; i < probs.size(); i ++) {
        if (probs[i] >= p0) {
            return iters[i] + a * iterPerRound;
        }
    }
    return (a + 1) * iterPerRound;;
}

void OPNL(){
    std::ofstream outFile("OPNL_out.txt");

    //freopen("find_best_c.txt", "w", stdout);
    std::cout << "Initializing" << std::endl; // Display the input value
    std::vector<int> iters {};
    std::vector<double> probs {};
    std::vector<int> best_iters {};
    std::vector<double> best_probs {};

    for (int M_pow = 0; M_pow <= 12; M_pow++) {
        int M = 1 << M_pow;
        outFile << "M = " << M << std::endl; // Display the input value
        std::cout << "M = " << M << std::endl; // Display the input value
        int bestTotalIter = INT_MAX;
        int bestRatio = 0;
        // increase delta_n qubits
        for (int ratio_pow = 1; ; ratio_pow += 1) {
            int ratio = 1 << ratio_pow; // eta
            int N = M * ratio;
            std::cout << "R = 1/" << ratio << std::endl;
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
            std::cout << "ratio= 1/" << ratio << ", N=" << N << ", bestAlpha=" << bestAlpha << ", Iter=" << bestIter << ", Prob=" << bestProb << std::endl;
            outFile << "ratio= 1/" << ratio << ", N=" << N << ", bestAlpha=" << bestAlpha << ", Iter=" << bestIter << ", Prob=" << bestProb << std::endl;
            int total_iter = calcTotalIter(bestProb, bestIter, best_iters, best_probs);
            // not searching for more ratio if total dueling rounds not increase anymore
            if (total_iter <  bestTotalIter) {
                bestTotalIter = total_iter;
                bestRatio = ratio;
            } else {
                break;
            }

        }
        outFile << "BestTotalIter is " << bestTotalIter << std::endl;
        outFile << "Best M to N ratio is 1/" << bestRatio << std::endl; 
    }
    outFile.close();
}

void P3SAT(){
    std::cout << "Initializing" << std::endl;
    std::ofstream outFile("P3SAT_out.txt");
    std::vector<int> iters {};
    std::vector<double> probs {};
    std::vector<int> best_iters {};
    std::vector<double> best_probs {};
    std::vector<std::vector<int>> total_iters_all(N_POW_BOUND + 1);
    outFile << std::endl;
    for (int rep = 0; rep < P3SAT_REPEAT_NUM; rep ++) {
        outFile << "------------------------------" <<std::endl;
        outFile << "Repeat #" << rep << std::endl;
        outFile << "------------------------------" << std::endl;
        iters.clear();
        probs.clear();
        best_iters.clear();
        best_probs.clear();
        for (int N_pow = 0; N_pow <= N_POW_BOUND; N_pow++) {
            int N = 1 << N_pow;
            outFile << "N = " << N << std::endl; // Display the input value
            std::cout << "N = " << N << std::endl; // Display the input value
            int bestTotalIter = INT_MAX;
            int bestRatio = 0;
            // increase delta_n qubits
            
            int M = static_cast<int>(ceil(sqrt(N)));
            std::unique_ptr<InitProblemInterface> initProblem = std::make_unique<PSATInit>();
            
            std::unique_ptr<Dueling> dueling = std::make_unique<Dueling>();
            dueling->setN(N);
            dueling->setInitProblemPtr(std::move(initProblem));
            
            dueling->setM(M);
            //static_cast<UniDistInit*>(dueling->getInitProblemPtr().get())->setStartIndex(N / M / 2);
            //M = (*initProblem).getActualM();
            dueling->initProblem();
                
            dueling->recordParameters();
            
            bool alpha_bound = true;
            int bestAlpha = 0, bestIter = 15;
            double bestProb = 0;

            for (int alpha = 1; alpha <= bestIter; alpha++) {
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
            std::cout << "Expected M=" << M << ", bestAlpha=" << bestAlpha << ", Iter=" << bestIter << ", Prob=" << bestProb << std::endl;
            outFile << "Expected M=" << M << ", bestAlpha=" << bestAlpha << ", Iter=" << bestIter << ", Prob=" << bestProb << std::endl;
            int total_iter = calcTotalIter(bestProb, bestIter, best_iters, best_probs);

            if (total_iter < 0) {
                throw std::runtime_error("Total iteration reaches neg inf");
                break;
            }
            total_iters_all[N_pow].push_back(total_iter);
            
            outFile << "BestTotalIter is " << total_iter << std::endl;
            outFile << std::endl;
        }
    }
    for (int N_pow = 0; N_pow <= N_POW_BOUND; N_pow++) {
        if (total_iters_all[N_pow].empty()) {
            std::cout << "Empty vector, no average" << std::endl;
            continue;
        }

        double sum = 0;
        for (int num : total_iters_all[N_pow]) {
            sum += num;
        }
        int N = 1 << N_pow;
        double average = sum / total_iters_all[N_pow].size();
        int avg = static_cast<int>(ceil(average));
        std::cout << "N=:" << N << ", avg=" << avg << std::endl;
        outFile << "N=:" << N << ", avg=" << avg << std::endl;
    }
    outFile.close();
}

void P3SAT_New() {
    std::cout << "Initializing" << std::endl;
    std::ofstream outFile("P3SAT_out.txt");
    std::vector<int> iters {};
    std::vector<double> probs {};
    std::vector<int> best_iters {};
    std::vector<double> best_probs {};
    std::vector<std::vector<int>> total_iters_all(N_POW_BOUND + 1);
    std::vector<std::vector<double>> max_probs_all(N_POW_BOUND + 1);
    outFile << std::endl;
    for (int rep = 0; rep < P3SAT_REPEAT_NUM; rep ++) {
        outFile << "------------------------------" <<std::endl;
        outFile << "Repeat #" << rep << std::endl;
        outFile << "------------------------------" << std::endl;
        iters.clear();
        probs.clear();
        best_iters.clear();
        best_probs.clear();
        for (int N_pow = 0; N_pow <= N_POW_BOUND; N_pow++) {
            int N = 1 << N_pow;
            outFile << "N = " << N << std::endl; // Display the input value
            std::cout << "N = " << N << std::endl; // Display the input value
            int bestTotalIter = INT_MAX;
            int bestRatio = 0;
            // increase delta_n qubits
            
            int M = static_cast<int>(floor(sqrt(N)));
            std::unique_ptr<InitProblemInterface> initProblem = std::make_unique<PSATInit>();
            
            std::unique_ptr<Dueling> dueling = std::make_unique<Dueling>();
            dueling->setN(N);
            dueling->setInitProblemPtr(std::move(initProblem));
            
            dueling->setM(M);
            //static_cast<UniDistInit*>(dueling->getInitProblemPtr().get())->setStartIndex(N / M / 2);
            //M = (*initProblem).getActualM();
            dueling->initProblem();
                
            dueling->recordParameters();
            
            bool alpha_bound = true;
            int bestAlpha = 0, bestIter = 15;
            double bestProb = 0;

            for (int alpha = 1; alpha <= bestIter; alpha++) {
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
            std::cout << "Expected M=" << M << ", bestAlpha=" << bestAlpha << ", Iter=" << bestIter << ", Prob=" << bestProb << std::endl;
            outFile << "Expected M=" << M << ", bestAlpha=" << bestAlpha << ", Iter=" << bestIter << ", Prob=" << bestProb << std::endl;
            
            total_iters_all[N_pow].push_back(bestIter);
            max_probs_all[N_pow].push_back(bestProb);
            
            outFile << "BestTotalIter to reach max probability is " << bestIter << std::endl;
            outFile << "MaxProb is " << bestProb << std::endl;
            outFile << std::endl;
        }
    }
    for (int N_pow = 0; N_pow <= N_POW_BOUND; N_pow++) {
        if (total_iters_all[N_pow].empty() || max_probs_all[N_pow].empty()) {
            std::cout << "Empty vector, no average" << std::endl;
            continue;
        }

        double sumIter = 0;
        for (int num : total_iters_all[N_pow]) {
            sumIter += num;
        }
        double sumProb = 0;
        for (double nump : max_probs_all[N_pow]) {
            sumProb += nump;
        }

        int N = 1 << N_pow;
        double averageIter = sumIter / total_iters_all[N_pow].size();
        int avgIter = static_cast<int>(ceil(averageIter));
        std::cout << "N=:" << N << ", avg BestIter=" << avgIter << std::endl;
        outFile << "N=:" << N <<  ", avg BestIter=" << avgIter << std::endl;

        double avgProb = sumProb / max_probs_all[N_pow].size();
        std::cout << "avg MaxProb=" << avgProb << std::endl;
        outFile << "avg MaxProb=" << avgProb << std::endl;
    }
    outFile.close();
}

/* Main function for OPNL and PSAT */
int main() {
    if (Problem == 0) {
        OPNL();
    } else if (Problem == 1) {
        P3SAT();
    } else {
        P3SAT_New();
    }
    return 0;
}