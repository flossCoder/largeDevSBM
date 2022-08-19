/*
 * main.cpp
 * Copyright (C) 2016, 2017 flossCoder
 *
 * This file is part of largeDevSBM.
 *
 * largeDevSBM is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * largeDevSBM is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>

#include "main.h"
#include "randomGenerator.h"

/**
 * The main function runs the whole program.
 *
 * @param argc Contains the number of given shell parameters. Default: 1
 * @param argv Contains a char-array with the given shell parameters.
 *             Default argv[0]: Name of the program. In this case the program
 *             runs the default simulation.
 *             Here one can start the following modes with additionally parameters:
 *             argv[1]: positive int number, mode to use (see *.), below)
 *             1.) Generate a set of SBM graphs and save them as graphviz files:
 *                 argv[2]: directory to save the graphviz files
 *                 argv[3]: name of the graphviz files
 *                 argv[4]: number of vertices
 *                 argv[5]: number of blocks
 *                 argv[6]: interblock connectivity
 *                 argv[7]: intrablock connectivity
 *                 argv[8]: seed
 */
int main(int argc, char** argv) {
    if (argc == 1) {
        // default case, just do the simulation
        doSimulation();
    } else if (atoi(argv[1]) == 1) {
        // generate some graphs and save them as a graphviz representation
        unsigned short seed = atoi(argv[8]);
        RandomGenerator *gen;
        gen = new RandomGenerator(seed);
        SBM *graph;
        graph = new SBM(atof(argv[6]), atof(argv[7]), atoi(argv[5]), atoi(argv[4]), gen, false, false);
        graph->generateRandom();
        graph->saveGraphToGraphviz(argv[2], argv[3]);
        std::cout << graph->getSizeOfLargestComponent() << std::endl;
    } else {
        // treat error case
        perror("Invalid given arguments");
        exit(EXIT_FAILURE);
    }

    return(0);
}
