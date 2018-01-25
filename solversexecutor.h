#ifndef SOLVERSEXECUTOR_H
#define SOLVERSEXECUTOR_H

#include <string>
#include <vector>
#include <iostream>

#include "solver.h"
#include "TSP.h"
#include "TSPSolution.h"

using namespace std;

class SolversExecutor
{
private:
    const char *mFilename;
    TSP mTspInstance;

    vector<Solver*> mSolvers;
    vector<TSPSolution*> mBestSolutions;

public:
    SolversExecutor(const char *filename);

    void addSolver(Solver* solver);

    void execute();

    void executeAndMeasureTime(Solver& tspSolver, TSPSolution& initSol, TSPSolution& bestSol);

    void printResults() const;
};

#endif // SOLVERSEXECUTOR_H
