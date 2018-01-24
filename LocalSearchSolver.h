/**
 * @file TSPSolver.h
 * @brief TSP solver (neighborhood search)
 *
 */

#ifndef LOCALSEARCHSOLVER_H
#define LOCALSEARCHSOLVER_H

#include <vector>

#include "TSPSolution.h"

/**
 * Class representing substring reversal move
 */
typedef struct move {
  int from;
  int to;
} TSPMove;



/**
 * Class that solves a TSP problem by neighbourdood search and 2-opt moves
 */
class LocalSearchSolver
{
public:
  LocalSearchSolver() {}

  /**
   * search for a good tour by neighbourhood search
   * @param TSP TSP data
   * @param initSol initial solution
   * @param bestSol best found solution (output)
   * @return true id everything OK, false otherwise
   */
  bool solve ( const TSP& tsp , const TSPSolution& initSol , TSPSolution& bestSol );

  /**
   * explore the neighbouhood
   * @param tsp TSP data
   * @param currSol center solution
   * @return (into param move) the selected move (stepest descent strategy)
   * @return the incremental cost with respect to currSol
   */
  double findBestNeighbor( const TSP& tsp , const TSPSolution& currSol , TSPMove& move );

  /**
   * perform a swap move (corresponding to 2-opt)
   * @param tspSol solution to be perturbed
   * @param move move to perform
   * @return (into param tspSol) the perturbed solution
   */
  TSPSolution& swap( TSPSolution& tspSol , const TSPMove& move );
};

#endif /* LOCALSEARCHSOLVER_H */
