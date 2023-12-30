#include "include/HeuristicDueling.h"
#include "include/InitMethods.h"

int main() {
    int N = 1 << 8;
    int M = 1 << 4;
    int DEPTH = 18;
    int CHANGE_LIMIT = 100;
    std::unique_ptr<InitProblemInterface> initProblem = std::make_unique<UniDistInit>();
    std::unique_ptr<HeuristicDueling> dueling = std::make_unique<HeuristicDueling>(N, M, std::move(initProblem), DEPTH, CHANGE_LIMIT);
    dueling->initProblem();
    dueling->recordParameters();
    dueling->initStateVector();
    for (int Iter = 1; Iter <= 7; Iter++) {
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
        }
        printf("\n");
    }
    return 0;
}