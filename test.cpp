#include "include/Dueling.h"
#include "include/InitMethods.h"

#include <cstdio>
#include <vector>

int main() {
    int N = 1 << 2;
    std::unique_ptr<InitProblemInterface> initProblem = std::make_unique<UniDistInit>();
    std::unique_ptr<Dueling> dueling = std::make_unique<Dueling>();
    dueling->setN(N);
    dueling->setInitProblemPtr(std::move(initProblem));
    dueling->setM(1);
    dueling->initProblem();
    dueling->recordParameters();
    dueling->setM(2);
    dueling->initProblem();
    dueling->recordParameters();
    return 0;
}