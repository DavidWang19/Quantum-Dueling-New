#pragma once


#include <vector>
#include <tuple>

// Forward declaration of the types if they are not defined elsewhere
using lit = std::tuple<int, int>;
using clause = std::tuple<lit, lit, lit>;

class PSAT {
public:
    // Member variables
    std::vector<clause> expression;
    std::vector<std::tuple<int, bool>> assignments;
    int numClause;
    int N;
    int EM;
    int M;

    PSAT(int initM, int initN); // Constructor


    PSAT(PSAT&&) = default;
    ~PSAT() = default;

private:
    // Private member variables or functions if any
};