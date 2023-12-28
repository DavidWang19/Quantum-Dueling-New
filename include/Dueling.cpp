#include "Dueling.h"

#include <cmath>
#include <algorithm>

Dueling::Dueling(int N, int M, std::unique_ptr<InitProblemInterface> initProblemPtr)
    : N(N), M(M), initProblemPtr(std::move(initProblemPtr))
{}

void Dueling::initProblem()
{
    initProblemPtr->initProblem(randomData, N, M);
    std::sort(randomData.begin(), randomData.end(), [](const RandomData& a, const RandomData& b) {
        if (a.value != b.value) return a.value < b.value;
        return a.isSolution < b.isSolution;
    });
}

void Dueling::recordParameters()
{
    clusterIsSol.clear();
    N_cnt.clear();
    clusterNum = 1;
    bool nowSolution = false;
    clusterIsSol.emplace_back(false);
    N_cnt.emplace_back(0);
    N_cnt.emplace_back(1);
    if (randomData[0].isSolution) {
        nowSolution = true;
        clusterIsSol.emplace_back(true);
    } else {
        clusterIsSol.emplace_back(false);
    }
    for (int i = 1; i < N; i++) {
        if (randomData[i].isSolution) {
            if (nowSolution) {
                if (randomData[i].value != randomData[i - 1].value) {
                    clusterNum++;
                    clusterIsSol.emplace_back(true);
                    N_cnt.emplace_back(0);
                }
            } else {
                clusterNum++;
                clusterIsSol.emplace_back(true);
                N_cnt.emplace_back(0);
            }
            nowSolution = true;
        } else {
            if (nowSolution) {
                nowSolution = false;
                clusterNum++;
                clusterIsSol.emplace_back(false);
                N_cnt.emplace_back(0);
            }
        }
        N_cnt[clusterNum]++;
    }
    phi.clear();
    phi.resize(clusterNum + 1, std::vector<double>(clusterNum + 1, 0));
    for (int k = 1; k <= clusterNum; k++) {
        for (int l = 1; l <= clusterNum; l++) {
            if (k < l && clusterIsSol[k]) {
                phi[k][l] = -1;
            } else {
                phi[k][l] = 1;
            }
        }
    }
    sol.clear();
    for (int i = 1; i <= clusterNum; i++) {
        if (clusterIsSol[i]) sol.emplace_back(i);
    }
}

void Dueling::initStateVector()
{
    psi.clear();
    psi.resize(clusterNum + 1, std::vector<double>(clusterNum + 1, 0));
    for (int k = 1; k <= clusterNum; k++) {
        for (int l = 1; l <= clusterNum; l++) {
            psi[k][l] = sqrt(N_cnt[k] * N_cnt[l]) / N;
        }
    }
}

void Dueling::G1()
{
    for (int l = 1; l <= clusterNum; l++) {
        double h = 0;
        for (int s = 1; s <= clusterNum; s++) {
            h += (sqrt(N_cnt[s] * 1.0 / N) * phi[s][l] * psi[s][l]);
        }
        for (int k = 1; k <= clusterNum; k++) {
            psi[k][l] = 2 * sqrt(N_cnt[k] * 1.0 / N) * h - phi[k][l] * psi[k][l];
        }
    }
}

void Dueling::G2()
{
    for (int k = 1; k <= clusterNum; k++) {
        double h = 0;
        for (int s = 1; s <= clusterNum; s++) {
            h += (sqrt(N_cnt[s] * 1.0 / N) * phi[s][k] * psi[k][s]);
        }
        for (int l = 1; l <= clusterNum; l++) {
            psi[k][l] = 2 * sqrt(N_cnt[l] * 1.0 / N) * h - phi[l][k] * psi[k][l];
        }
    }
}

double Dueling::getBestProb()
{
    double prob = 0;
    for (int i = 1; i <= clusterNum; i++) {
        prob += sqr(psi[sol[0]][i]);
        if (sol[0] != i) prob += sqr(psi[i][sol[0]]);
    }
    return prob;
}

std::vector<double> Dueling::getResultsAll()
{
    std::vector<double> resultsAll;
    resultsAll.clear();
    resultsAll.resize(clusterNum + 1, 0);
    for (int k = 1; k <= clusterNum; k++) {
        for (int l = 1; l <= clusterNum; l++) {
            if (clusterIsSol[k] == 1) {
                if (clusterIsSol[l] == 1) {
                    resultsAll[std::min(k, l)] += sqr(psi[k][l]);
                } else {
                    resultsAll[k] += sqr(psi[k][l]);
                }
            } else {
                if (clusterIsSol[l] == 1) {
                    resultsAll[l] += sqr(psi[k][l]);
                } else {
                    resultsAll[std::min(k, l)] += sqr(psi[k][l]);
                }
            }
        }
    }
    return resultsAll;
}