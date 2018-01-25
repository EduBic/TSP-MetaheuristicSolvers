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
        solversExe.addSolver(new TabuSearchSolver(5, 100));

        solversExe.execute();

        solversExe.printResults();

    }
    catch (std::exception& e) {
        std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
    }

    return 0;
}
