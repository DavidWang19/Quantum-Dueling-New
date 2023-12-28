#pragma once

#include "InitProblemInterface.h"

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