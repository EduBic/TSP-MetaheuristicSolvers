/**
 * @file TSPSolver.cpp
 * @brief TSP solver (neighborhood search)
 */

#include "TabuSearchSolver.h"
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <ctime>
#include <sys/time.h>

using namespace std;


std::string TabuSearchSolver::getSolverName() const {
    char buffer[50];
    sprintf(buffer, "\tTenure: %d, MaxIter: %d", mTabuLength, mMaxIteration);

    std::string tmp = std::string(buffer);

    if (ACmode) {
        if (BestImprovement) {
            return "Tabu Search AC - BI " + tmp;
        } else {
            return "Tabu Search AC - FI " + tmp;
        }
    } else {
        if (BestImprovement) {
            return "Tabu Search - BI " + tmp;
        } else {
            return "Tabu Search - FI " + tmp;
        }
    }
}

bool TabuSearchSolver::solve(const TSP& tsp, const TSPSolution& initSol, TSPSolution& bestSol) {

    // clear previous use
    //mTabuList.clear();
    mTabuSet.clear();
    mAdvTabuList.clear();

    try {
        bool stop = false;
        int  iter = 0;

        //tabuList.reserve(tsp.n);
        //initTabuList(tsp.n);

        TSPSolution currSol(initSol);
        double bestValue, currValue;

        bestValue = currValue = currSol.evaluateObjectiveFunction(tsp);
        TSPMove move;

        clock_t currTime = clock();

        while (!stop) {
            iter++;

            // for small instance of problem print the current solution
            if (tsp.n < 20) {
                currSol.print(std::cout);
                std::cout << " (" << iter << ") value " << currValue << "\t(" << bestValue << ")";
            }

            //mAspiration = bestValue - currValue;

            /*double costVar;
            if (BestImprovement) {
                costVar = findBestNeighbor(tsp, currSol, move);
            } else {
                costVar = findFirstBestNeighbor(tsp, currSol, move);
            }*/

            double bestNeighValue = currValue + findBestNeighbor(tsp, currSol, move); // costVar;

            if (bestNeighValue >= tsp.infinite) {
                cout << "\tmove: NO legal neighbour" << endl;
                stop = true;
            }
            else {
                // insertTabu(move);
                if (mTabuLength != 0) {

                    // use only List
                    /*if (mTabuList.size() < mTabuLength) {
                        mTabuList.push_back(move);
                    } else {
                        mTabuList.pop_front();  // remove old elements
                        mTabuList.push_back(move);
                    }*/

                    // use only List + Set
                    if (mTabuSet.size() < mTabuLength) {
                        // get key from TSPMove
                        char buffer[50];
                        sprintf(buffer, "%u%u", move.from, move.to);
                        string moveKey = string(buffer);

                        mAdvTabuList.push_back(moveKey);
                        mTabuSet.insert(moveKey);

                    } else {
                        // get key from TSPMove
                        char buffer[50];
                        sprintf(buffer, "%u%u", move.from, move.to);
                        string moveKey = string(buffer);

                        //cout << "\tNew tabu move: " << moveKey << "\t" << mTabuSet.size() << endl;

                        // remove old key
                        string oldTabuKey = mAdvTabuList.front();

                        mAdvTabuList.pop_front();
                        mTabuSet.erase(oldTabuKey);

                        // add new key
                        mAdvTabuList.push_back(moveKey);
                        mTabuSet.insert(moveKey);
                    }

                }

                currSol = swap(currSol,move);
                currValue = bestNeighValue;

                if (currValue < bestValue - 0.01) { // TS: update incumbent (if better -with tolerance- solution found)
                    bestValue = currValue;
                    bestSol = currSol;

                    cout << " (" << iter << ") value " << currValue
                         << "\tmove: " << move.from << " , " << move.to
                         << "\tbetter solution" << std::endl;
                }

                // stopping criteria
                /*if (iter > mMaxIteration) {
                    stop = true;
                } else*/ if ((double)(clock() - currTime) / CLOCKS_PER_SEC > mMaxTime) {
                    stop = true;
                }

                //std::cout << "\tmove: " << move.from << " , " << move.to;
                //std::cout << std::endl;
            }
        }

        bestSol.iterations = iter;

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

double TabuSearchSolver::findFirstBestNeighbor(const TSP& tsp , const TSPSolution& currSol, TSPMove& move) {
    double bestCostVariation = tsp.infinite;

    // N.B. intial and final position are fixed (initial/final node remains 0)

    // Slice all the current solution vector
    for (uint a = 1 ; a < currSol.sequence.size() - 2; a++) {

        // prev node
        int h = currSol.sequence[a-1];
        // choose a starting node
        int i = currSol.sequence[a];

        for (uint b = a + 1 ; b < currSol.sequence.size() - 1 ; b++) {

            int j = currSol.sequence[b];        // choose a finishing node
            int l = currSol.sequence[b+1];      // prev node

            // prof. implementation it discards other moves
            /*if ( ( (currIter - tabuList[i] <= tabuLength) &&
                    (currIter - tabuList[j] <= tabuLength)) ) continue */

            double neighCostVariation = - tsp.cost[h][i] - tsp.cost[j][l]
                                        + tsp.cost[h][j] + tsp.cost[i][l];


            if (isTabuMove(a, b) && !satisfiedAspirationCriteria(neighCostVariation)) {
                // DEBUG
                //std::cout << "\t" << "discard move: " << a << ", " << b << "\t";
            }
            else {

                // DEBUG
                /*if (isTabuMove(a, b) && satisfiedAspirationCriteria(neighCostVariation)) {
                    std::cout << "\t" << "in tabu but satisfied AC: " << a << ", " << b << "\t";
                }*/

                if (neighCostVariation < bestCostVariation) {
                    bestCostVariation = neighCostVariation;
                    move.from = a;
                    move.to = b;

                    double currentBestValueFound = currSol.evaluateObjectiveFunction(tsp);

                    // on first improvement exit
                    if (currentBestValueFound + bestCostVariation < currentBestValueFound) {
                        return bestCostVariation;
                    }

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


double TabuSearchSolver::findBestNeighbor( const TSP& tsp , const TSPSolution& currSol, TSPMove& move ) {
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

            int j = currSol.sequence[b];        // choose a finishing node
            int l = currSol.sequence[b+1];      // prev node

            // prof. implementation it discards other moves
            /*if ( ( (currIter - tabuList[i] <= tabuLength) &&
                    (currIter - tabuList[j] <= tabuLength)) ) continue */

            double neighCostVariation = - tsp.cost[h][i] - tsp.cost[j][l]
                                        + tsp.cost[h][j] + tsp.cost[i][l];


            if (!isTabuMove(a, b) /*|| satisfiedAspirationCriteria(neighCostVariation)*/) {
                if (neighCostVariation < bestCostVariation) {
                    bestCostVariation = neighCostVariation;
                    move.from = a;
                    move.to = b;
                }
            }

            /*if (isTabuMove(a, b) && !satisfiedAspirationCriteria(neighCostVariation)) {
                // DEBUG
                //std::cout << "\t" << "discard move: " << a << ", " << b << "\t";
            }
            else {

                // DEBUG
                if (isTabuMove(a, b) && satisfiedAspirationCriteria(neighCostVariation)) {
                    std::cout << "\t" << "in tabu but satisfied AC: " << a << ", " << b << "\t";
                }

                if (neighCostVariation < bestCostVariation) {
                    bestCostVariation = neighCostVariation;
                    move.from = a;
                    move.to = b;

                    //std::cout << "\t" << "better move: " << a << ", " << b << "\t";
                }
                else {
                    //std::cout << "\t" << "no better move: " << a << ", " << b << "\t";
                }
            }*/
        }
    }

    return bestCostVariation;
}

bool TabuSearchSolver::satisfiedAspirationCriteria(double neighbourCostVariation) const {
    if (ACmode) {
        // aspiration criteria implementation
        //std::cout << std::endl << "Aspiration criteria";
        return neighbourCostVariation < (mAspiration - 0.01);
    } else {
        return false; // default implementation
    }
}


bool TabuSearchSolver::isTabuMove(int from, int to) {
    /*std::list<TSPMove>::const_iterator it = mTabuList.begin();

    while (it != mTabuList.end()) {
        if ((*it).from == from && (*it).to == to)
            return true;
        it++;
    }*/


    char buffer[50];
    sprintf(buffer, "%u%u", from, to);
    string moveKey = string(buffer);

    if (mTabuSet.find(moveKey) != mTabuSet.end()) {
        return true;
    }

    return false;
}
