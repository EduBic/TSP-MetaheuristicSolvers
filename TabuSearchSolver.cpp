/**
 * @file TSPSolver.cpp
 * @brief TSP solver (neighborhood search)
 */

#include "TabuSearchSolver.h"
#include <iostream>

std::string TabuSearchSolver::getSolverName() const {
    return "Tabu Search";
}

bool TabuSearchSolver::solve(const TSP& tsp, const TSPSolution& initSol, TSPSolution& bestSol) {

    try {
        bool stop = false;
        int  iter = 0;

        // Tabu Search
        tabuList.reserve(tsp.n);
        //initTabuList(tsp.n);

        TSPSolution currSol(initSol);
        double bestValue, currValue;

        bestValue = currValue = currSol.evaluateObjectiveFunction(tsp);
        TSPMove move;

        while (!stop) {
            iter++;

            // for small instance of problem print the current solution
            if (tsp.n < 20) {
                currSol.print();
            }

            std::cout << " (" << iter << ") value " << currValue << "\t(" << currSol.evaluateObjectiveFunction(tsp) << ")";

            double bestNeighValue = currValue + findBestNeighbor(tsp, currSol, iter, move);

            if (bestNeighValue >= tsp.infinite) {                                                             // TS: stop because all neighbours are tabu
                std::cout << "\tmove: NO legal neighbour" << std::endl;
                stop = true;
            }
            else {
                std::cout << "\tmove: " << move.from << " , " << move.to;

                tabuList[currSol.sequence[move.from]] = iter;                                           // TS: update tabu list
                tabuList[currSol.sequence[move.to]]   = iter;

                // insertTabu(move);
                if (mTabuList.size() < mTabuLength) {
                    mTabuList.push_back(move);
                } else {
                    mTabuList.pop_front();  // remove old elements
                    mTabuList.push_back(move);
                }

                currSol = swap(currSol,move);                                                           // TS: always the best move
                currValue = bestNeighValue;

                if (currValue < bestValue - 0.01) {                                                   // TS: update incumbent (if better -with tolerance- solution found)
                    bestValue = currValue;
                    bestSol = currSol;
                    std::cout << "\tbetter solution";
                }

                // stopping criteria
                if (iter > mMaxIteration) {
                    stop = true;
                }

                std::cout << std::endl;
            }
        }

        return true;
    }
    catch(std::exception& e) {
        std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
        return false;
    }
}

TSPSolution& TabuSearchSolver::swap(TSPSolution& tspSol, const TSPMove& move) {

    TSPSolution tmpSol(tspSol);

    for ( int i = move.from ; i <= move.to ; ++i ) {
        tspSol.sequence[i] = tmpSol.sequence[move.to-(i-move.from)];
    }

    return tspSol;
}


double TabuSearchSolver::findBestNeighbor( const TSP& tsp , const TSPSolution& currSol , int currIter , TSPMove& move ) {    /// TS: use currIter
    // Determine the NON-TABU *move* yielding the best 2-opt neigbor solution
    double bestCostVariation = tsp.infinite;

    // N.B. intial and final position are fixed (initial/final node remains 0)

    // Slice all the current solution vector
    for (uint a = 1 ; a < currSol.sequence.size() - 2; a++) {

        // prev node
        int h = currSol.sequence[a-1];
        // choose a starting node
        int i = currSol.sequence[a];

        for (uint b = a + 1 ; b < currSol.sequence.size() - 1 ; b++) {

            // choose a finishing node
            int j = currSol.sequence[b];
            // prev node
            int l = currSol.sequence[b+1];

            // prof. implementation it discards other moves
            /*if ( ( (currIter - tabuList[i] <= tabuLength) &&
                    (currIter - tabuList[j] <= tabuLength)) ) {
                std::cout << "\t" << "discard move: " << a << ", " << b << "\t";*/

            if (isTabuMove(a, b)) {
                //std::cout << "\t" << "discard move: " << a << ", " << b << "\t";
            }
            else {

                double neighCostVariation = - tsp.cost[h][i] - tsp.cost[j][l]
                                            + tsp.cost[h][j] + tsp.cost[i][l];

                if (neighCostVariation < bestCostVariation) {
                    bestCostVariation = neighCostVariation;
                    move.from = a;
                    move.to = b;

                    //std::cout << "\t" << "better move: " << a << ", " << b << "\t";
                }
                else {
                    //std::cout << "\t" << "no better move: " << a << ", " << b << "\t";
                }
            }
        }
    }

    return bestCostVariation;
}


bool TabuSearchSolver::isTabuMove(int from, int to) {
    std::list<TSPMove>::const_iterator it = mTabuList.begin();

    while (it != mTabuList.end()) {

        //std::cout << "Hello" << std::endl << (*it).from << ", " << (*it).to << std::endl;

        if ((*it).from == from && (*it).to == to) return true;
        it++;
    }

    return false;
}
