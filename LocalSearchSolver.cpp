/**
 * @file TSPSolver.cpp
 * @brief TSP solver (neighborhood search)
 *
 */

#include "LocalSearchSolver.h"
#include <iostream>


bool LocalSearchSolver::solve( const TSP& tsp , const TSPSolution& initSol , TSPSolution& bestSol ) {

    try {
        bool stop = false;
        int  iter = 0;

        TSPSolution currSol(initSol);
        double bestValue, currValue;

        bestValue = currValue = currSol.evaluateObjectiveFunction(tsp);
        TSPMove move;

        while (!stop) {
            if ( tsp.n < 20 ) {
                currSol.print(); //log current solution (only small instances)
            }

            std::cout << " (" << ++iter << ") value " << currValue << " (" << bestValue << ")";

            // incremental evaluation: findBestNeighbour returns the cost increment
            double bestNeighValue = currValue + findBestNeighbor(tsp, currSol, move);
            std::cout << "\t move: " << move.from << " , " << move.to << std::endl;

            // stop criteria
            if (bestNeighValue < currValue) {
                bestValue = currValue = bestNeighValue;
                currSol = swap(currSol,move);
            }
            else {
                stop = true;    // exit from cycle
            }
        }

        bestSol = currSol;
    }
    catch (std::exception& e) {
        std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
        return false;
    }

    return true;
}

TSPSolution& LocalSearchSolver::swap( TSPSolution& tspSol , const TSPMove& move ) {

    TSPSolution tmpSol(tspSol);

    for (int i = move.from ; i <= move.to ; ++i) {
        tspSol.sequence[i] = tmpSol.sequence[move.to-(i-move.from)];
    }

    return tspSol;
}


double LocalSearchSolver::findBestNeighbor( const TSP& tsp , const TSPSolution& currSol , TSPMove& move ) {
    // Determine the *move* yielding the best 2-opt neigbor solution
    double bestCostVariation = tsp.infinite;

    // initial and final position are fixed (initial/final node remains 0)
    for ( uint a = 1 ; a < currSol.sequence.size() - 2 ; a++ ) {

        int h = currSol.sequence[a-1];
        int i = currSol.sequence[a];

        for ( uint b = a + 1 ; b < currSol.sequence.size() - 1 ; b++ ) {
            int j = currSol.sequence[b];
            int l = currSol.sequence[b+1];

            // incremental evaluation --> bestCostVariation (instead of best cost)
            double neighCostVariation = - tsp.cost[h][i] - tsp.cost[j][l]
                                        + tsp.cost[h][j] + tsp.cost[i][l];

            if ( neighCostVariation < bestCostVariation ) {
                bestCostVariation = neighCostVariation;
                move.from = a;
                move.to = b;
            }
        }
    }

    return bestCostVariation;
}








