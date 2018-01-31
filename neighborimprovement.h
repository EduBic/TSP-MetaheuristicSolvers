#ifndef NEIGHBORIMPROVEMENT
#define NEIGHBORIMPROVEMENT

#include <iostream>

#include "TSP.h"
#include "TSPSolution.h"
#include "solver.h"

using namespace std;

class NeigthborImprovement
{
public:
    /**
     * explore the neighbouhood
     * @param tsp TSP data
     * @param currSol center solution
     * @return (into param move) the selected move (stepest descent strategy)
     * @return the incremental cost with respect to currSol
     */
    virtual double execute(const TSP& tsp, const TSPSolution& currSol, TSPMove& move) = 0;

    virtual const string getName() const = 0;
};


class FirstImprovement : public NeigthborImprovement
{
public:

    double execute(const TSP& tsp , const TSPSolution& currSol, TSPMove& move) {
        //cout << endl << endl << "-------------- find neighbor --------------";


        double bestCostVariation = tsp.infinite;

        for ( uint a = 1 ; a < currSol.sequence.size() - 2 ; a++ ) {

            int h = currSol.sequence[a-1];
            int i = currSol.sequence[a];

            for ( uint b = a + 1 ; b < currSol.sequence.size() - 1 ; b++ ) {
                int j = currSol.sequence[b];
                int l = currSol.sequence[b+1];

                // incremental evaluation --> bestCostVariation (instead of best cost)
                double neighCostVariation = - tsp.cost[h][i] - tsp.cost[j][l]
                                            + tsp.cost[h][j] + tsp.cost[i][l];

                if (neighCostVariation < bestCostVariation ) {
                    // neighbor better of precedent neighbors
                    bestCostVariation = neighCostVariation;
                    move.from = a;
                    move.to = b;

                    double currentBestValueFound = currSol.evaluateObjectiveFunction(tsp);

                    // on first improvement exit
                    //cout << endl << "Move from " << a << " to " << b;
                    //cout << "\tNeighbor Value = " << currentBestValueFound + bestCostVariation << " - CurrentBestValue = " << currentBestValueFound;
                    if (currentBestValueFound + bestCostVariation < currentBestValueFound) {
                        //cout << "\tIt's an improvement, chose it!" << endl;
                        //cout << "------------------------" << endl;
                        return bestCostVariation;
                    }
                }
            }
        }

        //cout << endl << "------------------------------" << endl;
        return bestCostVariation;
    }

    const string getName() const {
        return "First Improvement";
    }
};


class BestImprovement : public NeigthborImprovement
{
public:
    double execute(const TSP &tsp, const TSPSolution &currSol, TSPMove &move) {
        //cout << endl << endl << "------------- find neighbor --------------";

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

                //cout << endl << "Move from " << a << " to " << b;
                //cout << "\tNeighbor Variation = " << neighCostVariation << " - BestCostVariation = " << bestCostVariation;

                if ( neighCostVariation < bestCostVariation ) {
                    //cout << "\tSave! Best neighbor found until now";

                    bestCostVariation = neighCostVariation;
                    move.from = a;
                    move.to = b;
                }
            }
        }

        //cout << endl << "-----------------------------" << endl;
        return bestCostVariation;
    }

    const string getName() const {
        return "Best Improvement";
    }
};


#endif // NEIGHBORIMPROVEMENT

