#include "solversexecutor.h"

#include <ctime>
#include <sys/time.h>

#include "TSPSolution.h"
#include "solver.h"

SolversExecutor::SolversExecutor(const char* filename) : mFilename(filename)
{
    mTspInstance.readFromFile(filename);
}

void SolversExecutor::addSolver(Solver *solver) {
    mSolvers.push_back(solver);
}

void SolversExecutor::execute() {
    // TODO: refactor init solutions
    TSPSolution aSolution(mTspInstance);
    aSolution.initRandom();

    for (std::vector<Solver*>::iterator it = mSolvers.begin() ; it != mSolvers.end(); ++it) {
        // new solution used by solver
        //TSPSolution bestSolution(mTspInstance);
        TSPSolution* bestSolution = new TSPSolution(mTspInstance);

        executeAndMeasureTime(*(*it), aSolution, *bestSolution);

        mBestSolutions.push_back(bestSolution);
    }
}

void SolversExecutor::executeAndMeasureTime(Solver& tspSolver, TSPSolution& initSol, TSPSolution& bestSol) {

    bestSol.solveBy = tspSolver.getSolverName();

    std::string tmp = "-----------------------------";
    std::cout << std::endl << tmp << tspSolver.getSolverName() << tmp << std::endl << std::endl;

    // initialize clocks for running time recording
    //   two ways:
    //   1) CPU time (t2 - t1)
    //   2) wall-clock time (tv2 - tv1)
    clock_t startClock, finishClock;
    timeval  tv1, tv2;

    startClock = clock();
    gettimeofday(&tv1, NULL);

    tspSolver.solve(mTspInstance, initSol, bestSol);

    finishClock = clock();
    gettimeofday(&tv2, NULL);

    cout << endl;
    cout << "in " << (double)(tv2.tv_sec+tv2.tv_usec*1e-6 - (tv1.tv_sec+tv1.tv_usec*1e-6)) << " seconds (user time)" << endl;
    cout << "in " << (double)(finishClock - startClock) / CLOCKS_PER_SEC << " seconds (CPU time)" << endl;
    cout << endl;
}

void SolversExecutor::printResults() const {

    for (vector<TSPSolution*>::const_iterator it = mBestSolutions.begin(); it != mBestSolutions.end(); ++it) {
        std::cout << std::endl << (*it)->solveBy << std::endl;
        (*it)->print();
        std::cout << "(value : " << (*it)->evaluateObjectiveFunction(mTspInstance) << ")\n";
    }

}

