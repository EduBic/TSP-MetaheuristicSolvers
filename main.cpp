/**
 * @file main.cpp
 * @brief 
 */


#include <stdexcept>
#include <ctime>
#include <sys/time.h>
#include <string>

#include "solver.h"
#include "LocalSearchSolver.h"
#include "TabuSearchSolver.h"
#include "solversexecutor.h"

// error status and messagge buffer
int status;
char errmsg[255];

using namespace std;


int main (int argc, char const *argv[]) {
    try {

        if (argc < 2) {
            throw std::runtime_error("usage: ./main filename.dat");
        }

        SolversExecutor solversExe(argv[1]);

        solversExe.addSolver(new LocalSearchSolver());
        solversExe.addSolver(new LocalSearchSolver(false));


        int maxIteration = 1000;

        for (int i = 3; i <= 45; i = i + 3) {
            solversExe.addSolver(new TabuSearchSolver(i, maxIteration, true));
            solversExe.addSolver(new TabuSearchSolver(i, maxIteration, true, false));

            solversExe.addSolver(new TabuSearchSolver(i, maxIteration));
            solversExe.addSolver(new TabuSearchSolver(i, maxIteration, false, false));
        }

        solversExe.execute();

        solversExe.printInitSolutions();
        solversExe.printResults();

    }
    catch (std::exception& e) {
        std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
    }

    return 0;
}
