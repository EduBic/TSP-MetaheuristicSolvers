/**
* @file TSPSolution.h
* @brief TSP solution
*
*/

#ifndef TSPSOLUTION_H
#define TSPSOLUTION_H

#include <vector>
#include <cmath>

#include "TSP.h"

/**
* TSP Solution representation: ordered sequence of nodes (path representation)
*/
class TSPSolution
{
public:
    std::vector<int> sequence;

    // utils solutions fields
    std::string solveBy;
    double userTime;
    double cpuTime;


    TSPSolution( const TSP& tsp ) {

        sequence.reserve(tsp.n + 1);

        for ( int i = 0; i < tsp.n ; ++i ) {
            sequence.push_back(i);
        }

        sequence.push_back(0);

        solveBy = "Random";
        userTime = -1.0;
        cpuTime = -1.0;
    }

    TSPSolution( const TSPSolution& tspSol ) {
        sequence.reserve(tspSol.sequence.size());

        for (uint i = 0; i < tspSol.sequence.size(); ++i ) {
            sequence.push_back(tspSol.sequence[i]);
        }
    }


    void initRandom(int seed = 42) {
        srand(seed/*time(NULL)*/);

        for (uint i = 1 ; i < sequence.size() ; ++i ) {
            // initial and final position are fixed (initial/final node remains 0)

            int idx1 = rand() % (sequence.size()-2) + 1;
            int idx2 = rand() % (sequence.size()-2) + 1;

            int tmp = sequence[idx1];
            sequence[idx1] = sequence[idx2];
            sequence[idx2] = tmp;
        }

        std::cout << "###" << std::endl;
        print(std::cout);
        std::cout << "###" << std::endl;
    }

    double evaluateObjectiveFunction(const TSP& tsp ) const {
        double total = 0.0;

        for ( uint i = 0 ; i < sequence.size() - 1 ; ++i ) {
            int from = sequence[i]  ;
            int to   = sequence[i+1];
            total += tsp.cost[from][to];
        }

        return total;
    }


    void print(std::ostream& out) const {
        out << std::endl;
        for (uint i = 0; i < sequence.size(); i++ ) {
            out << sequence[i] << " ";
        }
        out << std::endl;
    }

    TSPSolution& operator=(const TSPSolution& right) {

        // Handle self-assignment:
        if(this == &right) {
            return *this;
        }
        else { // if (this != &right)
            for (uint i = 0; i < sequence.size(); i++ ) {
                sequence[i] = right.sequence[i];
            }
        }

        return *this;
    }
};

#endif /* TSPSOLUTION_H */
