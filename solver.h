#ifndef SOLVER
#define SOLVER

#include <string>

#include "TSP.h"
#include "TSPSolution.h"

/**
 * Class representing substring reversal move
 */
typedef struct move {
  int from;
  int to;
} TSPMove;


class Solver {
public:

    virtual std::string getSolverName() const = 0;

    virtual bool solve(const TSP& tsp, const TSPSolution& initSol, TSPSolution& bestSol) = 0;

};

#endif // SOLVER

