/**
 * @file TSPSolver.cpp
 * @brief TSP solver (neighborhood search)
 */

#include "LocalSearchSolver.h"
#include <iostream>

std::string LocalSearchSolver::getSolverName() const {
    return string("Local Search ") + findNeighbor->getName();
}

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

            // first improvement
            // neigh = findFirstBestNeighbor(tsp, currSol, )
            // neighValue = compute neight obj fun value
            // if neighValue < currentBest then bestFound = currentBest = neightValue


            // incremental evaluation: findBestNeighbour returns the cost increment
            double bestNeighValue = currValue + findNeighbor->execute(tsp, currSol, move);

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







