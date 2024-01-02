#include "p3sat.h"

#include <cmath>
#include <random>
#include <string>
#include <iostream>

#define NUM_LITERAL_PER_CLAUSE 3

/* Determine whether an assignment satisfy F1, the first part of a expression */
bool PSAT_sat(int a, int upperbound, const std::vector<clause> &expression) {
    for (size_t i = 0; i < upperbound; i ++) {
        lit lit1 = std::get<0> (expression[i]);
        lit lit2 = std::get<1> (expression[i]);
        
        lit lit3 = std::get<2> (expression[i]);
        if (((a >> (std::get<1> (lit1))) & 1) != (std::get<0> (lit1)) &&
            ((a >> (std::get<1> (lit2))) & 1) != (std::get<0> (lit2)) &&
            ((a >> (std::get<1> (lit3))) & 1) != (std::get<0> (lit3)))
            return false;
    }
    
    return true;
}

/* Determine how many clauses an assignment satisfy  in F2, the second part of a expression */
int PSAT_val(int a, int lowerbound, const std::vector<clause> &expression) {
    int count = 0;
    for (size_t i = lowerbound; i < expression.size(); i ++) {
        lit lit1 = std::get<0> (expression[i]);
        lit lit2 = std::get<1> (expression[i]);
        lit lit3 = std::get<2> (expression[i]);
        if (((a >> (std::get<1> (lit1))) & 1) == (std::get<0> (lit1)) ||
            ((a >> (std::get<1> (lit2))) & 1) == (std::get<0> (lit2)) ||
            ((a >> (std::get<1> (lit3))) & 1) == (std::get<0> (lit3)))
            count += 1;
    }
    
    return count;
}

/* Generate a PMAX-3SAT expression, and precompute whether an assignment is a solution, how optimized 
 * it is as a solution, for all possible assignments' solution 
 */
PSAT::PSAT(int initM, int initN) : EM(initM), N(initN) {
    M = EM;
    /* For each assignment, the probability that it not satisfy a clause is 1/8,
     * (7/8)^x N = M
     * numClause = log_(7/8)(M/N) = log_(7/8)(1/sqrt(N))
     * 
     */
    double psatRatio = 1.0 - (1.0 / (1 << NUM_LITERAL_PER_CLAUSE));
    numClause = static_cast<int> (ceil(log((static_cast<double> (EM))/N) / log(psatRatio)));
    
    std::cout << "numClause  = " << numClause << std::endl;
    size_t half_size = static_cast<size_t> (numClause);
    numClause = numClause * 2; // half restrict, half for values
    if (numClause < 0) numClause = 0;
    // Initialize asssignments
    
    int nbits = static_cast<int> (ceil(log2(N))); // 1 bits for each atomic literal in SAT
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, nbits - 1);

    // Random number generator using the Mersenne Twister engine
    std::mt19937 rng(std::random_device{}());
    // Uniform distribution for integers, with only two possible values: 0 and 1
    std::uniform_int_distribution<int> dist1(0, 1);

    
    for (size_t i = 0; i < numClause; i ++) {
        lit lit1 = std::make_tuple(dist1(rng), dist(mt));
        lit lit2 = std::make_tuple(dist1(rng), dist(mt));
        lit lit3 = std::make_tuple(dist1(rng), dist(mt));
        expression.push_back(std::make_tuple(lit1, lit2, lit3));
    }
    //assignments.resize(N);
    std::cout << "Almost Done " << std::endl;
    for (size_t a = 0; a < N; a ++) {
        assignments.push_back(std::make_tuple(PSAT_val(a, half_size, expression), 
                                         PSAT_sat(a, half_size, expression)));
    }
    
}
