/*
 * main.h
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

#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <iostream>
#include <stdlib.h>

#include "mcPresenter.h"
#include "graphParameter.h"
#include "randomGenerator.h"

GraphParameter *param;
MCPresenter *sim;

/**
 * Test the given string for equality to "y" and "n".
 *
 * @param str, the given string.
 *
 * @return true: str == "y", false: str == "n"
 *
 * @exception invalid given string
 */
bool isTrue(std::string str) {
    if (str.compare("y") == 0) {
        return(true);
    }
    if (str.compare("n") == 0) {
        return(false);
    }
    perror(("invalid string " + str).c_str());
    exit(EXIT_FAILURE);
}

/**
 * Function for reading in an directory.
 *
 * @return string, which contains the directory for saving all files of the simulation.
 */
std::string enterDirectory() {
    std::string directory;
    //std::cout << "enter directory for the results" << std::endl;
    std::cin >> directory;
    return(directory);
}

/**
 * Enter the temperature.
 *
 * @return the temperature.
 */
double enterTemperature() {
    double temp;
    //std::cout << "enter the artificial temperature" << std::endl;
    std::cin >> temp;
    return(temp);
}

/**
 * Enter the number of samples.
 *
 * @return the number of samples.
 */
int enterNumberOfSamples(std::string input) {
    int samples;
    //std::cout << "enter the number of samples for " << input << std::endl;
    std::cin >> samples;
    return(samples);
}

/**
 * Set up the system required for doing a simulation.
 */
void setup() {
    std::string helper;
    int n, generator, graph, value;
    unsigned short int seed;
    param = new GraphParameter();

    //std::cout << "is digraph y/n" << std::endl;
    std::cin >> helper; //################################################################################################ s1
    if (isTrue(helper)) {
        param->isDigraph = true;
    } else {
        param->isDigraph = false;
    }

    //std::cout << "are loops allowed y/n" << std::endl;
    std::cin >> helper; //################################################################################################ s2
    if (isTrue(helper)) {
        param->loopsAllowed = true;
    } else {
        param->loopsAllowed = false;
    }

    helper = enterDirectory(); //######################################################################################### s3

    //std::cout << "enter the number of vertices of the graph" << std::endl;
    std::cin >> n; //##################################################################################################### s4

    //std::cout << "enter the number of the generator to use:" << std::endl;
    //std::cout << "  0: default generator" << std::endl;
    std::cin >> generator; //############################################################################################# s5

    //std::cout << "enter a seed for the random number generator:" << std::endl;
    std::cin >> seed; //################################################################################################## s6

    //std::cout << "enter the value to use:" << std::endl;
    //std::cout << "  0: largest component" << std::endl;
    std::cin >> value; //################################################################################################# s7

    //std::cout << "enter the number of the graph to use:" << std::endl;
    //std::cout << "  0: ER graphs" << std::endl;
    //std::cout << "  1: stochastic block model graphs" << std::endl;
    std::cin >> graph; //################################################################################################# s8

    if (graph == 0) {
        // Erdos-Renyi graphs
        //std::cout << "enter connectivity of the graph" << std::endl;
        std::cin >> param->connectivity; //############################################################################### s9
    } else if (graph == 1) {
        // Stochastic blockmodel
        //std::cout << "enter interblock connectivity of the graph" << std::endl;
        std::cin >> param->connectivity; //############################################################################### s9
        //std::cout << "enter intrablock connectivity of the graph" << std::endl;
        std::cin >> param->secondConnectivity; //######################################################################### s10
        //std::cout << "enter the number of blocks" << std::endl;
        std::cin >> param->numberOfBlocks; //############################################################################# s11
    } else {
        perror("Invalid graph " + graph);
        exit(EXIT_FAILURE);
    }

    sim = new MCPresenter(param, helper, n, seed, generator, graph, value);
}

/**
 * Perform simple sampling.
 */
void simpleSampling() {
    int samples = enterNumberOfSamples("simple sampling");
    sim->simpleSampling(samples); //###################################################################################### ss1
}

/**
 * Use the Metropolis algorithm.
 */
void metropolisAlgorithm() {
    double temp;
    int samples, equi;
    temp = enterTemperature(); //######################################################################################### ma1
    samples = enterNumberOfSamples("importance sampling"); //############################################################# ma2
    equi = enterNumberOfSamples("equilibration steps"); //################################################################ ma3
    sim->metropolisAlgorithm(temp, samples, equi);
}

/**
 * Calculate the histograms for determining the number of steps
 * until the simulation is equilibrated.
 */
void equilibrate() {
    double temp;
    int samples;
    temp = enterTemperature(); //######################################################################################### eq1
    samples = enterNumberOfSamples("calculating the number of equilibration steps"); //################################### eq2
    sim->calculateEquilibrationSteps(temp, samples);
}

/**
 * Use the Wang-Landau algorithm.
 */
void wangLandauAlgorithm() {
    int lowerBound, upperBound, sweepsToEvaluate;
    double modiFacFinal;
    //std::cout << "enter lower bound" << std::endl;
    std::cin >> lowerBound; //############################################################################################ wl1
    //std::cout << "enter upper bound" << std::endl;
    std::cin >> upperBound; //############################################################################################ wl2
    //std::cout << "enter the number of sweeps for the evaluation" << std::endl;
    std::cin >> sweepsToEvaluate; //###################################################################################### wl3
    //std::cout << "enter the final modification factor" << std::endl;
    std::cin >> modiFacFinal; //########################################################################################## wl4
    sim->wangLandauAlgorithm(lowerBound, upperBound, sweepsToEvaluate, modiFacFinal);
}

/**
 * Choose the action which has to be done.
 */
void chooseAction() {
    int action;
    //std::cout << "which action should be done" << std::endl;
    //std::cout << "  0: simple sampling" << std::endl;
    //std::cout << "  1: Metropolis algorithm" << std::endl;
    //std::cout << "  2: equilibrate" << std::endl;
    //std::cout << "  3: Wang-Landau algorithm" << std::endl;
    std::cin >> action; //############################################################################################### c1
    if (action == 0) {
        simpleSampling();
    } else if (action == 1) {
        metropolisAlgorithm();
    } else if (action == 2) {
        equilibrate();
    } else if (action == 3) {
        wangLandauAlgorithm();
    }
}

/**
 * Do the whole simulation.
 */
void doSimulation() {
    setup();
    chooseAction();
    delete param;
    delete sim;
}

#endif // MAIN_H_INCLUDED
