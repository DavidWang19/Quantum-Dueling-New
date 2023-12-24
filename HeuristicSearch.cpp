#include "include/HeuristicDueling.h"
#include "include/InitMethods.h"

int main() {
    int N = 1 << 8;
    int M = 1 << 4;
    int DEPTH = 7;
    int CHANGE_LIMIT = 5;
    double PROB_TH = 0.4;
    std::unique_ptr<InitProblemInterface> initProblem = std::make_unique<UniDistInit>();
    std::unique_ptr<HeuristicDueling> dueling = std::make_unique<HeuristicDueling>(N, M, std::move(initProblem), DEPTH, CHANGE_LIMIT);
    dueling->initProblem();
    dueling->recordParameters();
    dueling->initStateVector();
    for (int Iter = 1;; Iter++) {
        bool breakFlag = false;
        printf("Iteration #%d:\n", Iter);
        auto ret = dueling->getNextBestGateArr();
        for (int i = 0; i < DEPTH; i++) {
            if (ret[i] == 1) {
                dueling->G1();
                printf("G1 ");
            } else {
                dueling->G2();
                printf("G2 ");
            }
            double prob = dueling->getBestProb();
            printf("%.8lf\n", prob);
            if (prob > PROB_TH) {
                printf("\nTarget probability is achieved after %d Dueling gates!\n", (Iter - 1) * DEPTH + i + 1);
                breakFlag = true;
                break;
            }
        }
        if (breakFlag) break;
        printf("\n");
    }
    return 0;
}