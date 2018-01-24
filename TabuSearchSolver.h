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

#include "TSPSolution.h"
#include "LocalSearchSolver.h"  // need for the struct TSPMove

/**
 * Class that solves a TSP problem by neighbourdood search and 2-opt moves
 */
class TabuSearchSolver
{
public:
    int tabuLength;
    std::vector<int> tabuList;  // prof tabu list: unused

    std::list<TSPMove> mTabuList;




    TabuSearchSolver() {}

    bool solve( const TSP& tsp , const TSPSolution& initSol , int tabulength , int maxIter , TSPSolution& bestSol ); /// TS: new parameters

    double findBestNeighbor( const TSP& tsp , const TSPSolution& currSol , int currIter , TSPMove& move );        /// TS: use currIter!!!

    TSPSolution& swap(TSPSolution& tspSol , const TSPMove& move );

    // Tabu search (tabu list stores, for each node, when (last iteration) a move involving that node have been chosen)
    // a neighbor is tabu if the generating move involves two nodes that have been chosen in the last 'tabulength' moves
    // that is, currentIteration - LastTimeInvolved <= tabuLength
    void initTabuList(int n) {
        for ( int i = 0 ; i < n ; ++i ) {
            // at iterarion 0, no neighbor is tabu --> iteration(= 0) - tabulistInit > tabulength --> tabulistInit < tabuLength + 0
            tabuList.push_back(-tabuLength-1);
        }
    }

    bool isTabuMove(int from, int to);

};

#endif /* TSPSOLVER_H */
