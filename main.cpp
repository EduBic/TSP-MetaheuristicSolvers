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
            throw std::runtime_error("usage: ./main <filename>.dat");
        }

        SolversExecutor solversExe(argv[1]);

        //solversExe.addRandomSeedInitSolution(42);
        //solversExe.addRandomSeedInitSolution(58);
        solversExe.addRandomSeedInitSolution(46);
        solversExe.addRandomSeedInitSolution(93);

        //solversExe.addSolver(new LocalSearchSolver());
        //solversExe.addSolver(new LocalSearchSolver(false));

        //solversExe.addSolver(new TabuSearchSolver(100, 100000, true, true, 60*5));


        int maxIteration = 100000;
        double maxSeconds = 20;//60 * 1;

        //solversExe.addSolver(TabuSearchSolver::buildTS_BI(100, maxIteration, maxSeconds));
        //solversExe.addSolver(TabuSearchSolver::buildTS_FI(i, maxIteration, maxSeconds));

        for (int i = 100; i <= 100; i = i + 10) {
            solversExe.addSolver(TabuSearchSolver::buildTS_BI(i, maxIteration, maxSeconds));
            //solversExe.addSolver(TabuSearchSolver::buildTS_BI_AC(i, maxIteration, maxSeconds));
            //solversExe.addSolver(TabuSearchSolver::buildTS_FI(i, maxIteration, maxSeconds));
            //solversExe.addSolver(TabuSearchSolver::buildTS_FI_AC(i, maxIteration, maxSeconds));
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
