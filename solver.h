#ifndef SOLVER
#define SOLVER

class Solver {
public:
    bool solve(const TSP& tsp , const TSPSolution& initSol , TSPSolution& bestSol ) = 0;
};

#endif // SOLVER

