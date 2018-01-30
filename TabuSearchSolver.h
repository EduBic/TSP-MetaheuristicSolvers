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

#include "solver.h"

using namespace std;

class TSStopCriteria {

};

/**
 * Class that solves a TSP problem by neighbourdood search and 2-opt moves
 */
class TabuSearchSolver : public Solver
{
public:
    int tabuLength;
    vector<int> tabuList;  // prof tabu list: unused

    uint mTabuLength;
    int mMaxIteration;
    double mMaxTime;
    list<TSPMove> mTabuList;

    double mAspiration;
    // config variable
    bool ACmode;
    bool BestImprovement;


    //TSStopCriteria StopCriteria;



    TabuSearchSolver() {}

    TabuSearchSolver(int tabuLength, int maxIter, bool aspCriteria = false, bool bestImprovement = true, double maxSeconds = 1e10)
        : mTabuLength(tabuLength), mMaxIteration(maxIter), ACmode(aspCriteria), BestImprovement(bestImprovement), mMaxTime(maxSeconds) {}

    // Factory methods
    static TabuSearchSolver* buildTS_BI(int tabuLenght, int maxIter, double maxSeconds = 1e10) {
        return new TabuSearchSolver(tabuLenght, maxIter, false, true, maxSeconds);
    }

    static TabuSearchSolver* buildTS_BI_AC(int tabuLenght, int maxIter, double maxSeconds = 1e10) {
        return new TabuSearchSolver(tabuLenght, maxIter, true, true, maxSeconds);
    }

    static TabuSearchSolver* buildTS_FI(int tabuLenght, int maxIter, double maxSeconds = 1e10) {
        return new TabuSearchSolver(tabuLenght, maxIter, false, false, maxSeconds);
    }

    static TabuSearchSolver* buildTS_FI_AC(int tabuLenght, int maxIter, double maxSeconds = 1e10) {
        return new TabuSearchSolver(tabuLenght, maxIter, true, false, maxSeconds);
    }


    std::string getSolverName() const;

    bool solve(const TSP &tsp, const TSPSolution &initSol, TSPSolution &bestSol);

    bool satisfiedAspirationCriteria(double neighbourCostVariation) const;


    // private:

    double findBestNeighbor(const TSP& tsp , const TSPSolution& currSol , TSPMove& move );
    double findFirstBestNeighbor(const TSP &tsp, const TSPSolution &currSol, TSPMove &move);

    TSPSolution& swap(TSPSolution& tspSol , const TSPMove& move );

    bool isTabuMove(int from, int to);

};

#endif /* TSPSOLVER_H */
