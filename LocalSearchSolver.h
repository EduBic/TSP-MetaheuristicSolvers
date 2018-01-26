/**
 * @file TSPSolver.h
 * @brief TSP solver (neighborhood search)
 *
 */

#ifndef LOCALSEARCHSOLVER_H
#define LOCALSEARCHSOLVER_H

#include <vector>

#include "solver.h"
#include "neighborimprovement.h"



/**
 * Class that solves a TSP problem by neighbourdood search and 2-opt moves
 */
class LocalSearchSolver : public Solver
{
public:
    NeigthborImprovement* findNeighbor;

    LocalSearchSolver(bool bestImprovement = true) {
        if (bestImprovement) {
            findNeighbor = new BestImprovement();
        } else {
            findNeighbor = new FirstImprovement();
        }
    }

  /**
   * search for a good tour by neighbourhood search
   * @param TSP TSP data
   * @param initSol initial solution
   * @param bestSol best found solution (output)
   * @return true id everything OK, false otherwise
   */
  bool solve ( const TSP& tsp , const TSPSolution& initSol , TSPSolution& bestSol );

  std::string getSolverName() const;


  /**
   * perform a swap move (corresponding to 2-opt)
   * @param tspSol solution to be perturbed
   * @param move move to perform
   * @return (into param tspSol) the perturbed solution
   */
  TSPSolution& swap( TSPSolution& tspSol , const TSPMove& move );
};

#endif /* LOCALSEARCHSOLVER_H */
