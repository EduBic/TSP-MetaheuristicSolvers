/**
 * @file main.cpp
 * @brief 
 */


#include <stdexcept>
#include <ctime>
#include <sys/time.h>
#include <string>

#include "LocalSearchSolver.h"
#include "TabuSearchSolver.h"

// error status and messagge buffer
int status;
char errmsg[255];

using namespace std;

void printSolution(TSPSolution& solution, TSP& tspInstance, string title) {
    std::cout << std::endl << title << std::endl;
    solution.print();
    std::cout << "(value : " << solution.evaluateObjectiveFunction(tspInstance) << ")\n";
}

void printTime(clock_t& t1, clock_t& t2, timeval& tv1, timeval& tv2) {
    cout << endl;
    cout << "in " << (double)(tv2.tv_sec+tv2.tv_usec*1e-6 - (tv1.tv_sec+tv1.tv_usec*1e-6)) << " seconds (user time)" << endl;
    cout << "in " << (double)(t2-t1) / CLOCKS_PER_SEC << " seconds (CPU time)" << endl;
}


int main (int argc, char const *argv[]) {
    try {

        if (argc < 2) {
            throw std::runtime_error("usage: ./main filename.dat");
        }

        // (1) init phase
        TSP tspInstance;
        tspInstance.readFromFile(argv[1]);

        // init solution
        TSPSolution aSolution(tspInstance);
        aSolution.initRandom();

        // initialize clocks for running time recording
        //   two ways:
        //   1) CPU time (t2 - t1)
        //   2) wall-clock time (tv2 - tv1)
        clock_t startClock, finishClock;
        timeval  tv1, tv2;

        // (2) solver phase

        startClock = clock();
        gettimeofday(&tv1, NULL);

        LocalSearchSolver tspSolver;

        // run the neighbourhood search
        TSPSolution bestSolution(tspInstance);
        tspSolver.solve(tspInstance, aSolution, bestSolution);

        finishClock = clock();
        gettimeofday(&tv2, NULL);

        printTime(startClock, finishClock, tv1, tv2);


        cout << endl;
        cout << "------ TABU SEARCH -----" << endl;
        cout << endl;


        startClock = clock();
        gettimeofday(&tv1, NULL);

        int tabuLength = 30; // atoi(argv[2]);
        int maxIter    = 10000; // atoi(argv[3]);

        TabuSearchSolver tsSolver;
        TSPSolution bestSolution2(tspInstance);
        tsSolver.solve(tspInstance, aSolution, tabuLength, maxIter, bestSolution2);

        finishClock = clock();
        gettimeofday(&tv2, NULL);

        printTime(startClock, finishClock, tv1, tv2);

        printSolution(aSolution, tspInstance, "Init solution");
        printSolution(bestSolution, tspInstance, "Local search solution");
        printSolution(bestSolution2, tspInstance, "Tabu search solution");

    }
    catch (std::exception& e) {
        std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
    }

    return 0;
}
