#include "HeuristicDueling.h"

HeuristicDueling::HeuristicDueling(int N, int M, std::unique_ptr<InitProblemInterface> initProblemPtr, int DEPTH, int CHANGE_LIMIT)
    : Dueling(N, M, std::move(initProblemPtr))
{
    this->DEPTH = DEPTH;
    this->CHANGE_LIMIT = CHANGE_LIMIT;
}

void HeuristicDueling::initStateVector()
{
    Dueling::initStateVector();
    psiRef = psi;
    gateArr.clear();
    gateArr.resize(DEPTH, 0);
}

std::vector<int>& HeuristicDueling::getNextBestGateArr()
{
    psiRef = psi;
    bestProb = -1;
    nextStep(0, CHANGE_LIMIT);
    psi = psiRef;
    return bestGateArr;
}

void HeuristicDueling::nextStep(int dep, int remainChange)
{
    if (dep == DEPTH) {
        psi = psiRef;
        for (int i = 0; i < DEPTH; i++) {
            if (gateArr[i] == 1) {
                G1();
            } else {
                G2();
            }
        }
        double prob = getBestProb();
        if (prob > bestProb) {
            bestProb = prob;
            bestGateArr = gateArr;
        }
        return;
    }
    if (dep == 0) {
        gateArr[dep] = 1;
        nextStep(dep + 1, remainChange);
        gateArr[dep] = 2;
        nextStep(dep + 1, remainChange);
        return;
    }
    if (gateArr[dep - 1] == 2) {
        if (remainChange > 0) {
            gateArr[dep] = 1;
            nextStep(dep + 1, remainChange - 1);
        }
        gateArr[dep] = 2;
        nextStep(dep + 1, remainChange);
    } else {
        gateArr[dep] = 1;
        nextStep(dep + 1, remainChange);
        if (remainChange > 0) {
            gateArr[dep] = 2;
            nextStep(dep + 1, remainChange - 1);
        }
    }
}