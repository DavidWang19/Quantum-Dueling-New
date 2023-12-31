#pragma once

#include "InitProblemInterface.h"
#include "p3sat.h"

/* Initialize OPNL Problem */
class OPNLInit : public InitProblemInterface
{
    public:
        OPNLInit() = default;
        OPNLInit(OPNLInit&&) = default;
        ~OPNLInit() = default;

        virtual void initProblem(std::vector<DataPoint>& randomData, int N, int M) override;
        void setStartIndex(int startIndex) { this->startIndex = startIndex; }
    private:
        int startIndex = 0;
};

/* Initialize PMAX-3SAT Problem */
class PSATInit : public InitProblemInterface
{
    public:
        PSATInit() = default;
        PSATInit(PSATInit&&) = default;
        ~PSATInit() = default;
        
        virtual void initProblem(std::vector<DataPoint>& randomData, int N, int M) override;
        void setStartIndex(int startIndex) { this->startIndex = startIndex; }
    private:
        int startIndex = 0;
};