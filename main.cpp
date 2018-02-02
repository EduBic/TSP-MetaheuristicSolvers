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

        // Test initial solutions
        solversExe.addRandomSeedInitSolution(58);
        solversExe.addRandomSeedInitSolution(4);
        solversExe.addRandomSeedInitSolution(25);
        solversExe.addRandomSeedInitSolution(26);
        solversExe.addRandomSeedInitSolution(43);
        solversExe.addRandomSeedInitSolution(46);
        solversExe.addRandomSeedInitSolution(93); 
        solversExe.addRandomSeedInitSolution(99);

        solversExe.addSolver(new LocalSearchSolver());
        solversExe.addSolver(new LocalSearchSolver(false));

        //solversExe.addSolver(new TabuSearchSolver(100, 2000, true, true, 10));


        int maxIteration = 100000;
        double maxSeconds = 30;
        //int bestTabuLenght = 180;   // for rnd100.dat

        //solversExe.addSolver(TabuSearchSolver::buildTS_BI(bestTabuLenght, maxIteration, maxSeconds));
        //solversExe.addSolver(TabuSearchSolver::buildTS_BI_AC(bestTabuLenght, maxIteration, maxSeconds));
        //solversExe.addSolver(TabuSearchSolver::buildTS_FI(bestTabuLenght, maxIteration, maxSeconds));
        //solversExe.addSolver(TabuSearchSolver::buildTS_FI_AC(bestTabuLenght, maxIteration, maxSeconds));



        //for (int i = 6000; i <= 14000; i = i + 2000) {
            for (int t = 80; t <= 200; t = t + 20) {
                //solversExe.addSolver(TabuSearchSolver::buildTS_BI(t, maxIteration, maxSeconds));
                //solversExe.addSolver(TabuSearchSolver::buildTS_BI_AC(t, maxIteration, maxSeconds));
                //solversExe.addSolver(TabuSearchSolver::buildTS_FI(t, maxIteration, maxSeconds));
                //solversExe.addSolver(TabuSearchSolver::buildTS_FI_AC(t, maxIteration, maxSeconds));
            }
        //}

        solversExe.execute();

        solversExe.printInitSolutions();
        solversExe.printResults();

    }
    catch (std::exception& e) {
        std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
    }

    return 0;
}
