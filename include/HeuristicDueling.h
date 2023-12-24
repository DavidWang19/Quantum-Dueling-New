#include "Dueling.h"

class HeuristicDueling : public Dueling
{
    public:
        HeuristicDueling(int N, int M, std::unique_ptr<InitProblemInterface> initProblemPtr, int DEPTH, int CHANGE_LIMIT);
        HeuristicDueling(HeuristicDueling&&) = default;
        ~HeuristicDueling() = default;

        void initStateVector();
        std::vector<int>& getNextBestGateArr();
    private:
        int DEPTH, CHANGE_LIMIT;
        std::vector<std::vector<double>> psiRef;
        std::vector<int> gateArr, bestGateArr;
        double bestProb;

        void nextStep(int dep, int remainChange);
};