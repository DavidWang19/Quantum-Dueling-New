#pragma once

#include <vector>

struct DataPoint {
    int value;
    bool isSolution;
};

class InitProblemInterface
{
    public:
        virtual void initProblem(std::vector<DataPoint>& randomData, int N, int M) = 0;
};