#include "solversexecutor.h"

#include <fstream>
#include <ctime>
#include <sys/time.h>
#include <typeinfo>

#include "TSPSolution.h"
#include "TabuSearchSolver.h"
#include "solver.h"

using namespace std;

SolversExecutor::SolversExecutor(const char* filename) : mFilename(filename)
{
    mTspInstance.readFromFile(filename);
}

void SolversExecutor::addRandomSeedInitSolution(int seed) {
    TSPSolution* initSol = new TSPSolution(mTspInstance);
    initSol->initRandom(seed);

    mInitSolutions.push_back(initSol);
}

void SolversExecutor::addRandomInitSolution() {
    addRandomSeedInitSolution(time(NULL));
}

void SolversExecutor::addSolver(Solver *solver) {
    mSolvers.push_back(solver);
}

void SolversExecutor::execute() {

    // name log file with actual date
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer),"%d-%m-%Y %I:%M:%S", timeinfo);
    string nowTime(buffer);

    string fileNameStr = string(mFilename);
    fileNameStr.erase(0, 5);

    ofstream outputLog("Log/" + fileNameStr + "-outputLog-" + nowTime + ".txt");
    ofstream latexLog("Log/" + fileNameStr + "-latexLog-" + nowTime + ".txt");
    //ofstream csvLog("Log/" + fileNameStr + "-csvLog-" + nowTime + ".csv");

    //csvLog << "Length, Avg. Value, Avg. Time, Best Value found, Total time" << endl;

    latexLog << "Solver, Avg Value, Best Value found, Avg. Time, Total time, Avg Iter" << endl;


    for (std::vector<Solver*>::iterator it = mSolvers.begin(); it != mSolvers.end(); ++it) {

        latexLog << endl;

        int i = 0;
        double values[mInitSolutions.size()];
        double times[mInitSolutions.size()];
        uint iterations[mInitSolutions.size()];
        double bestOfBestvalue = 1e10;

        for (std::vector<TSPSolution*>::iterator inIt = mInitSolutions.begin(); inIt != mInitSolutions.end(); ++inIt) {

            TSPSolution* bestSolution = new TSPSolution(mTspInstance);
            executeAndMeasureTime(*(*it), **inIt, *bestSolution);

            mBestSolutions.push_back(bestSolution);


            // print solution into log file
            double value = bestSolution->evaluateObjectiveFunction(mTspInstance);

            if (value < bestOfBestvalue) {
                bestOfBestvalue = value;
            }

            // print logconsole result of init solution solved
            outputLog << std::endl << "----------------------------------------------------------------------" << std::endl
                      << std::endl << bestSolution->solveBy << std::endl;

            bestSolution->print(outputLog);

            outputLog << "(value : " << value << ")\t"
                      << "sec. (user time) " << bestSolution->userTime << "\t"
                      << "sec. (CPU time) " << bestSolution->cpuTime << std::endl;

            // print latex result of init solution solved
            //latexLog << bestSolution->solveBy << ", " << value << ", " << bestSolution->cpuTime << endl;

            // for compute average
            values[i] = value;
            times[i] = bestSolution->cpuTime;
            iterations[i] = bestSolution->iterations;

            i++;
        }


        double sumValue = 0;
        double sumTime = 0;
        uint sumIter = 0;
        for (unsigned int j = 0; j < mInitSolutions.size(); j++) {
            sumValue += values[j];
            sumTime += times[j];
            sumIter += iterations[j];
        }

        double avgValue = sumValue / mInitSolutions.size();
        double avgTime = sumTime / mInitSolutions.size();
        double avgIter = sumIter / mInitSolutions.size();

        //latexLog << endl;
        //latexLog << "Solver, Avg Value, Avg. Time, Best Value found, Total time, Avg Iter" << endl;
        latexLog << (*it)->getSolverName()
                 << ", " << avgValue
                 << ", " << bestOfBestvalue
                 << ", " << avgTime
                 << ", " << sumTime
                 << ", " << avgIter;
        latexLog << endl;


        // Print result for Calibration of TS
        TabuSearchSolver* tsSolver = dynamic_cast<TabuSearchSolver*>(*it);
        if (tsSolver != NULL) {
            //csvLog << tsSolver->mTabuLength << ", " << avgValue << ", " << ", " << bestOfBestvalue << ", " << avgTime << ", " << sumTime << endl;
        }

        // Print result
        // csvLog << (*it)->getSolverName() << ", " << bestOfBestvalue << ", " << sumTime << endl;
    }

    //csvLog.close();
    latexLog.close();
    outputLog.close();
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

    bestSol.userTime = (double)(tv2.tv_sec+tv2.tv_usec*1e-6 - (tv1.tv_sec+tv1.tv_usec*1e-6));
    bestSol.cpuTime = (double)(finishClock - startClock) / CLOCKS_PER_SEC;
}

void SolversExecutor::printInitSolutions() const {
    for (vector<TSPSolution*>::const_iterator it = mInitSolutions.begin(); it != mInitSolutions.end(); ++it) {
        std::cout << std::endl << (*it)->solveBy << std::endl;
        std::cout << "(value : " << (*it)->evaluateObjectiveFunction(mTspInstance) << ")\n";
        std::cout << "sec. (user time) " << (*it)->userTime << std::endl;
        std::cout << "sec. (CPU time) " << (*it)->cpuTime << std::endl;
    }
}

void SolversExecutor::printResults() const {
    std::cout << "----------------------------------------------------------------------" << std::endl;

    TSPSolution* bestSolutionFound;
    std::vector<TSPSolution*> bestSolutionsFound;
    double bestValue = 1e10;

    for (vector<TSPSolution*>::const_iterator it = mBestSolutions.begin(); it != mBestSolutions.end(); ++it) {
        double value = (*it)->evaluateObjectiveFunction(mTspInstance);

        if (value < bestValue) {
            bestValue = value;
            bestSolutionFound = *it;
        }

        if (value <= bestValue) {
            bestSolutionsFound.push_back(*it);
        }

        std::cout << std::endl << (*it)->solveBy << std::endl;
        std::cout << "(value : " << value << ")\n";
        std::cout << "sec. (user time) " << (*it)->userTime << std::endl;
        std::cout << "sec. (CPU time) " << (*it)->cpuTime << std::endl;
    }

    if (mBestSolutions.size() > 1) {

        std::cout << "------------------------------- THE WINNER -------------------------------------" << std::endl;

        std::cout << std::endl << bestSolutionFound->solveBy << std::endl;
        std::cout << "(value : " << bestValue << ")\n";
        std::cout << "sec. (user time) " << bestSolutionFound->userTime << std::endl;
        std::cout << "sec. (CPU time) " << bestSolutionFound->cpuTime << std::endl;

        std::cout << "------------------------------- THE WINNERS -------------------------------------" << std::endl;

        for (vector<TSPSolution*>::const_iterator it = bestSolutionsFound.begin(); it != bestSolutionsFound.end(); ++it) {
            double value = (*it)->evaluateObjectiveFunction(mTspInstance);

            std::cout << std::endl << (*it)->solveBy << std::endl;
            std::cout << "(value : " << value << ")\n";
            std::cout << "sec. (user time) " << (*it)->userTime << std::endl;
            std::cout << "sec. (CPU time) " << (*it)->cpuTime << std::endl;
        }
    }

    cout << endl;
}

