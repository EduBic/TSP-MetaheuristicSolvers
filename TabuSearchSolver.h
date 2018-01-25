/**
 * @file TSPSolver.h
 * @brief TSP solver (neighborhood search)
 *
 */

#ifndef TSPSOLVER_H
#define TSPSOLVER_H

#include <vector>
#include <list>
#include <iostream>

//#include "TSPSolution.h"
#include "LocalSearchSolver.h"  // need for the struct TSPMove
#include "solver.h"

/**
 * Class that solves a TSP problem by neighbourdood search and 2-opt moves
 */
class TabuSearchSolver : public Solver
{
public:
    int tabuLength;
    std::vector<int> tabuList;  // prof tabu list: unused

    int mTabuLength;
    int mMaxIteration;
    std::list<TSPMove> mTabuList;

    double mAspiration;
    // config variable
    bool ACmode;



    TabuSearchSolver() {}

    TabuSearchSolver(int tabuLength, int maxIter, bool aspCriteria = false) : mTabuLength(tabuLength), mMaxIteration(maxIter), ACmode(aspCriteria) {}


    std::string getSolverName() const;

    bool solve(const TSP &tsp, const TSPSolution &initSol, TSPSolution &bestSol);

    bool satisfiedAspirationCriteria(double neighbourCostVariation) const;


    // private:

    double findBestNeighbor( const TSP& tsp , const TSPSolution& currSol , int currIter , TSPMove& move );

    TSPSolution& swap(TSPSolution& tspSol , const TSPMove& move );

    bool isTabuMove(int from, int to);

};

#endif /* TSPSOLVER_H */
