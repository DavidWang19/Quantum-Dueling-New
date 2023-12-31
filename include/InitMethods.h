#pragma once

#include "InitProblemInterface.h"

class UniDistInit : public InitProblemInterface
{
    public:
        UniDistInit(int startIndex = 0) : startIndex(startIndex) {}
        UniDistInit(UniDistInit&&) = default;
        ~UniDistInit() = default;

        virtual void initProblem(std::vector<DataPoint>& randomData, int N, int M) override;
        void setStartIndex(int startIndex) { this->startIndex = startIndex; }
    private:
        int startIndex = 0;
};

class RandomInit : public InitProblemInterface
{
    public:
        RandomInit() = default;
        RandomInit(int seed) : seed(seed) {}
        RandomInit(RandomInit&&) = default;
        ~RandomInit() = default;

        virtual void initProblem(std::vector<DataPoint>& randomData, int N, int M) override;
    private:
        int seed = -1;
};

class FullSquareInit : public InitProblemInterface
{
    public:
        FullSquareInit() = default;
        FullSquareInit(FullSquareInit&&) = default;
        ~FullSquareInit() = default;

        virtual void initProblem(std::vector<DataPoint>& randomData, int N, int M) override;
};

class WorstCaseInit : public InitProblemInterface
{
    public:
        WorstCaseInit() = default;
        WorstCaseInit(WorstCaseInit&&) = default;
        ~WorstCaseInit() = default;

        virtual void initProblem(std::vector<DataPoint>& randomData, int N, int M) override;
};