#pragma once

#include "InitProblemInterface.h"

class UniDistInit : public InitProblemInterface
{
    public:
        UniDistInit(int startIndex = 0) : startIndex(startIndex) {}
        UniDistInit(UniDistInit&&) = default;
        ~UniDistInit() = default;

        virtual void initProblem(std::vector<DataPoint>& randomData, int N, int M) override;
    private:
        int startIndex = 0;
};

class RandomInit : public InitProblemInterface
{
    public:
        RandomInit() = default;
        RandomInit(RandomInit&&) = default;
        ~RandomInit() = default;

        virtual void initProblem(std::vector<DataPoint>& randomData, int N, int M) override;
};