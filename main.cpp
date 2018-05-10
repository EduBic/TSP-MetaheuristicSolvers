/**
 * @file main.cpp
 * @brief 
 */


#include <stdexcept>
#include <string>

#include <getopt.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "solver.h"
#include "LocalSearchSolver.h"
#include "TabuSearchSolver.h"
#include "solversexecutor.h"

// error status and messagge buffer
int status;
char errmsg[255];

using namespace std;

/** Struct containing the long options */
static struct option long_options[] = {
    {"ls", no_argument, NULL, 'l'},             // Local Search
    {"ts", no_argument, NULL, 't'},             // Tabu Search

    {"fi", no_argument, NULL, 'f'},             // First Improvement
    {"bi", no_argument, NULL, 'b'},             // Best Improvement

    {"ac", no_argument, NULL, 'a'},             // Aspiration Criteria

    {"maxIter", required_argument, NULL, 'i'},  // Max iteration for TS
    {"tenure", required_argument, NULL, 'e'},   // Tenure for TS
    {"secs", required_argument, NULL, 's'},     // Seconds for TS

    {"bm", required_argument, NULL, 'm'},       // Benchmark
    {0, 0, 0, 0}
};


int main (int argc, char *argv[]) {
    try {

        if (argc < 2) {
            throw std::runtime_error("usage: ./main <filename>.dat");
        }

        SolversExecutor solversExe(argv[1]);

        bool benchmark = false;

        // Solver option    default = LocalSearch
        bool localSearch = true;

        // Solvers features     default = BI
        bool bestImprove = true;
        bool aspCriteria = false;   // only for TabuSearch

        // Tabu options
        int seconds = 30;
        int tenure = 50;
        int maxIterations = 1000;

        int c;
        int option_index;

        while((c = getopt_long(argc, argv, "lfbtae:i:s:m", long_options, &option_index)) != EOF) {
            switch(c) {
                case 'l': {
                    localSearch = true;
                    break;
                }
                case 't': {
                    localSearch = false;
                    break;
                }
                case 'b': {
                    bestImprove = true;
                    break;
                }
                case 'f': {
                    bestImprove = false;
                    break;
                }
                case 'a': {
                    aspCriteria = true;
                    cout << endl << "AspCriteria: " << aspCriteria << endl;
                    break;
                }
                case 'e': {
                    tenure = strtol(optarg, NULL, 0);
                    cout << endl << "Tenure: " << tenure << endl << endl;
                    break;
                }
                case 'i': {
                    maxIterations = (int)strtol(optarg, NULL, 0);
                    break;
                }
                case 's': {
                    seconds = (int)strtol(optarg, NULL, 0);
                    break;
                }
                case 'm': {
                    benchmark = true;
                    break;
                }
            }
        }

        if (benchmark) {

            // Test initial solutions
            solversExe.addRandomSeedInitSolution(58);
            solversExe.addRandomSeedInitSolution(4);
            solversExe.addRandomSeedInitSolution(25);
            solversExe.addRandomSeedInitSolution(26);
            solversExe.addRandomSeedInitSolution(43);
            solversExe.addRandomSeedInitSolution(46);
            solversExe.addRandomSeedInitSolution(93);
            solversExe.addRandomSeedInitSolution(99);

            //solversExe.addSolver(new LocalSearchSolver());
            //solversExe.addSolver(new LocalSearchSolver(false));


            int maxIteration = 300000;
            double maxSeconds = 10;
            //int tabuLenght = 600;
            int tabuLengths[4] = {0, 20, 180, 480};

            //solversExe.addSolver(TabuSearchSolver::buildTS_BI(tabuLenght, maxIteration, maxSeconds));

            for (int i = 0; i < 4; i++) {
                solversExe.addSolver(TabuSearchSolver::buildTS_BI(tabuLengths[i], maxIteration, maxSeconds));
                //solversExe.addSolver(TabuSearchSolver::buildTS_BI_AC(tabuLengths[i], maxIteration, maxSeconds));
                //solversExe.addSolver(TabuSearchSolver::buildTS_FI(tabuLengths[i], maxIteration, maxSeconds));
                //solversExe.addSolver(TabuSearchSolver::buildTS_FI_AC(tabuLengths[i], maxIteration, maxSeconds));
            }

        } else {
            // Command line program
            solversExe.addRandomInitSolution();

            if (localSearch) {
                solversExe.addSolver(new LocalSearchSolver(bestImprove));
            } else { // tabu search
                solversExe.addSolver(new TabuSearchSolver(tenure, maxIterations, aspCriteria, bestImprove, seconds));
            }
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
