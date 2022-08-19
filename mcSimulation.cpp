/*
 * mcSimulation.cpp
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

#include "mcSimulation.h"

/**
 * Constructor for setting up the simulation.
 *
 * @param d, where the file should be saved.
 * @param presenter, the preseter of the simulation.
 */
MCSimulation::MCSimulation(std::string d, MCPresenter *presenter) {
    directory = d;
    simulationPresenter = presenter;
    histogram = new Histogram(simulationPresenter->getNumberOfVertices());
}

/**
 * Destructor.
 */
MCSimulation::~MCSimulation() {
    delete histogram;
}

/**
 * Generate a set of graphs and calculate a histogram.
 *
 * @param numberOfGraphs, how many graphs should be generated randomly.
 */
void MCSimulation::simpleSampling(int numberOfGraphs) {
    AbstractGraph *graph = simulationPresenter->generateGraph(GENERATEEMPTYGRAPH);
    double result;
    std::ofstream outfile;
    outfile.open((directory + "/" + "ss_" + numToStr(simulationPresenter->getNumberOfVertices()) + "_"
                                          + numToStr(numberOfGraphs) + ".dat").c_str());
    for (int i = 0; i < numberOfGraphs; i++) {
        graph->generateRandom(); // generate a randomly drawn graph
        result = simulationPresenter->getValueOfInterest(graph);
        outfile << i << " " << result << " ";
        simulationPresenter->printStatistics(outfile, graph);
        outfile << std::endl;
        histogram->increment(result);
    }
    outfile.close();

    histogram->saveHistogram(directory, "hist_ss_" + numToStr(simulationPresenter->getNumberOfVertices()) + "_"
                                                   + numToStr(numberOfGraphs));
    delete graph;
}

/**
 * Perform the simulation on one graph under one starting condition
 * using the Metropolis algorithm.
 *
 * @param temperature, the artificial temperature, make sure temperature != 0.
 * @param sweeps, how many sweeps should be recorded.
 * @param equilibrationSweeps, number of steps to perform, until the simulation is equilibrated.
 */
void MCSimulation::metropolisAlgorithm(double temperature, int sweeps, int equilibrationSweeps) {
    int i;
    double result;
    parameterCollector_type parameters;
    parameters.temperature = temperature;
    int numberOfVertices = simulationPresenter->getNumberOfVertices();
    AbstractGraph *graph = simulationPresenter->generateGraph(GENERATERANDOMGRAPH); // always start with an random graph
    // equilibrate system
    for (i = 0; i < numberOfVertices * equilibrationSweeps; i++) {
        MCSimulation::generateNextState(USEMETROPOLISALGORITHM, graph, parameters);
    }
    // open outfile
    std::ofstream outfile;
    outfile.open((directory + "/" + "is_" + numToStr(numberOfVertices) + "_"
                                          + numToStr(sweeps) + "_"
                                          + numToStr(temperature) + ".dat").c_str());
    // do the simulation
    for (i = 0; i < numberOfVertices * sweeps; i++) {
        MCSimulation::generateNextState(USEMETROPOLISALGORITHM, graph, parameters);
        // save each numberOfVertices step
        if (i % numberOfVertices == 0) {
            result = simulationPresenter->getValueOfInterest(graph);
            outfile << (i + 1) / numberOfVertices << " " << result << " ";
            simulationPresenter->printStatistics(outfile, graph);
            outfile << std::endl;
            histogram->increment(result);
        }
    }
    outfile.close();

    histogram->saveHistogram(directory, "hist_is_" + numToStr(numberOfVertices) + "_"
                                                   + numToStr(sweeps) + "_"
                                                   + numToStr(temperature));

    delete graph;
}

/**
 * Calculate the number of steps required to equilibrate the simulation
 *
 * @param temperature, the artificial temperature, make sure temperature != 0.
 * @param equilibrationSweeps, the number of sweeps to perform for this calculation.
 */
void MCSimulation::calculateEquilibrationSteps(double temperature, int equilibrationSweeps) {
    int steps = 0;
    parameterCollector_type parameters;
    parameters.temperature = temperature;
    int numberOfVertices = simulationPresenter->getNumberOfVertices();

    // declare the graph variables
    AbstractGraph *completeGraph;
    AbstractGraph *lineGraph;
    AbstractGraph *randomGraph;
    AbstractGraph *emptyGraph;

    // initialize the graph variables
    completeGraph = simulationPresenter->generateGraph(GENERATECOMPLETEGRAPH);
    lineGraph = simulationPresenter->generateGraph(GENERATELINEGRAPH);
    randomGraph = simulationPresenter->generateGraph(GENERATERANDOMGRAPH);
    emptyGraph = simulationPresenter->generateGraph(GENERATEEMPTYGRAPH);

    // save the values of the largest components for each graph
    double completeLC = simulationPresenter->getValueOfInterest(completeGraph);
    double lineLC = simulationPresenter->getValueOfInterest(lineGraph);
    double randomLC = simulationPresenter->getValueOfInterest(randomGraph);
    double emptyLC = simulationPresenter->getValueOfInterest(emptyGraph);

    // prepare data output agreed format: sweep empty complete line random
    std::ofstream outfile;
    outfile.open((directory + "/" + "equiExperiment_" + numToStr(numberOfVertices) + "_"
                                                      + numToStr(equilibrationSweeps) + "_"
                                                      + numToStr(temperature) + ".dat").c_str());
    // save initial sweep
    outfile << ((double) steps) / ((double) numberOfVertices) << " " << completeLC << " " << lineLC << " " << randomLC << " " << emptyLC << std::endl;

    while (steps < numberOfVertices * equilibrationSweeps){
        // do next step
        MCSimulation::generateNextState(USEMETROPOLISALGORITHM, completeGraph, parameters);
        MCSimulation::generateNextState(USEMETROPOLISALGORITHM, lineGraph, parameters);
        MCSimulation::generateNextState(USEMETROPOLISALGORITHM, randomGraph, parameters);
        MCSimulation::generateNextState(USEMETROPOLISALGORITHM, emptyGraph, parameters);
        // save largest component values
        completeLC = simulationPresenter->getValueOfInterest(completeGraph);
        lineLC = simulationPresenter->getValueOfInterest(lineGraph);
        randomLC = simulationPresenter->getValueOfInterest(randomGraph);
        emptyLC = simulationPresenter->getValueOfInterest(emptyGraph);

        steps++;
        // write the measured values to output
        if (steps % numberOfVertices == 0) {
            outfile << ((double) steps) / ((double) numberOfVertices) << " " << completeLC << " " << lineLC << " " << randomLC << " " << emptyLC << std::endl;
        }
    }

    delete completeGraph;
    delete lineGraph;
    delete randomGraph;
    delete emptyGraph;
}

/**
 * Perform the simulation on one graph in the given intervall until the
 * flattness criterium is fullfilled.
 *
 * @param lowerBound, the minimum value for accepting the step.
 * @param upperBound, the maximum value for accepting the step.
 * @param sweepsToEvaluate, the number of sweeps after that the
 *        histogram check is performed.
 * @param modiFacFinal, the final modification factor.
 */
void MCSimulation::wangLandauAlgorithm(int lowerBound, int upperBound, int sweepsToEvaluate, double modiFacFinal) {
    int numberOfVertices = simulationPresenter->getNumberOfVertices();
    double density[numberOfVertices];
    for (int i = 0; i < numberOfVertices; i++) {
        density[i] = log(1.0);
    }
    // set the parameters:
    parameterCollector_type parameters;
    parameters.density = density;       // pointer to density array
    parameters.lowerBound = lowerBound; // lower bound of the interval
    parameters.upperBound = upperBound; // upper bound of the interval

    double modiFac = log(exp(1.0)); // the modification factor
    double value = 0.0;
    AbstractGraph *graph = simulationPresenter->generateGraph(GENERATEEMPTYGRAPH); // always start with an empty graph
    // insert randomly new edges, until the size of the largest component is bigger than the lower bound
    while (!(lowerBound < simulationPresenter->getValueOfInterest(graph))) {
        graph->generateRandomEdge();
    }

    int evalTime = (numberOfVertices * sweepsToEvaluate); // the number of steps after the histogram should be evaluated
    bool saturated = false; // true, if the modification factor once dropped under t^-1
    // some boolean parameters to make the semantics of the code clearer
    // one could write them into the if () conditions
    bool isEvalSweep = false;
    bool isSweep = false;
    bool modiFacLessTime = false;

    std::ofstream outfile;
    outfile.open((directory + "/" + "wl_" + numToStr(simulationPresenter->getNumberOfVertices()) + "_"
                                          + numToStr(lowerBound) + "_" + numToStr(upperBound) + "_"
                                          + numToStr(sweepsToEvaluate) + "_" + numToStr(modiFacFinal) + ".dat").c_str());

    unsigned long long int step = 0;
    // execute the algorithm
    while (modiFac >= modiFacFinal) {
        step += 1;
        // calculate next state
        MCSimulation::generateNextState(USEWANGLANDAUALGORITHM, graph, parameters);
        // update densities and histogram
        value = simulationPresenter->getValueOfInterest(graph);
        density[((int) value - 1)] = density[((int) value - 1)] + (modiFac);
        histogram->increment(value);
        if (step % (100 * numberOfVertices) == 0) {
            outfile << (step + 1) / numberOfVertices << " " << value << " ";
            simulationPresenter->printStatistics(outfile, graph);
            outfile << std::endl;
        }

        // test, whether the histogram must be reseted
        // prepare boolean parameters
        isEvalSweep = (step % evalTime == 0); // correct step?
        isSweep = (step % numberOfVertices == 0);
        // is the modification factor less than or equals t^-1?
        modiFacLessTime = (modiFac <= (1.0 / ((double) step / (double) numberOfVertices)));
        if (isEvalSweep &&
            not(modiFacLessTime) && not(saturated) && // modification factor bigger than t^-1?
            (histogram->areAllBinsNonZero(lowerBound, upperBound))) { // is the all bins non zero condition fullfilled?
            // reduce the modification factor
            modiFac = modiFac / 2.0;
            if (modiFac >= modiFacFinal) {
                histogram->resetHistogram();
            }
        } else if (isSweep && (saturated || modiFacLessTime)) {
            // check each MC sweep, whether the modification factor must be set to t^-1
            saturated = true;
            modiFac = (1.0 / ((double) step / (double) numberOfVertices));
        }
    }

    histogram->saveHistogram(directory, "hist_wl_" + numToStr(numberOfVertices) + "_"
                                                   + numToStr(lowerBound) + "_"
                                                   + numToStr(upperBound));
    MCSimulation::saveDensity(density, directory, numberOfVertices, lowerBound, upperBound, step);

    outfile.close();

    // clean up
    delete graph;
}

/**
 * Generate the next state for the simulation. This function generates the candidate graph,
 * calculates the size of the largest components for both graphs and sets the next state.
 *
 * @param algorithm, the used algorithm for importance sampling.
 * @param currentState, pointer to the current state of the graph.
 * @param parameters, the required parameters for the accept / reject decision.
 */
void MCSimulation::generateNextState(int algorithm, AbstractGraph *currentState,
                                      parameterCollector_type parameters) {
    // 1. save size of the largest component of the current state
    double currentValue = simulationPresenter->getValueOfInterest(currentState);
    // 2. generate the candidate graph
    int *vertex;
    vertex = new int;
    previousState safedState = currentState->generateCandidateGraph(vertex);
    // 3. accept the candidate graph with the metropolis probability
    if (!MCSimulation::acceptNewState(algorithm, currentValue, simulationPresenter->getValueOfInterest(currentState), parameters)) {
        currentState->revertGenerateCandidateGraph(*vertex, safedState); // reject candidate
    }

    // clean up
    delete vertex;
}

/**
 * Decide whether to accept or reject the new state.
 *
 * @param algorithm, the used algorithm for importance sampling.
 * @param currentValue, value for the current state.
 * @param candidateValue, value for the candidate graph.
 * @param parameters, the required parameters for the accept / reject decision.
 *
 * @return true: accept the candidate graph, false: reject the candidate graph.
 *
 * @exception Invalid algorithm number.
 */
bool MCSimulation::acceptNewState(int algorithm, double currentValue, double candidateValue,
                                   parameterCollector_type parameters) {
    if (algorithm == USEMETROPOLISALGORITHM) {
        // use the Metropolis algorithm
        double exponent = (candidateValue - currentValue) / parameters.temperature;
        double metropolisProbability = std::min(1.0, exp(-exponent));
        return (simulationPresenter->randomNumber() <= metropolisProbability);
    } else if (algorithm == USEWANGLANDAUALGORITHM) {
        // use the Wang-Landau algorithm
        double exponent = ((*(parameters.density + ((int) currentValue - 1))) / (*(parameters.density + ((int) candidateValue - 1))));
        double aP = std::min(1.0, exp(exponent)); // acceptance probability
        return ((simulationPresenter->randomNumber() <= aP)
                 && (candidateValue >= parameters.lowerBound)
                 && (candidateValue <= parameters.upperBound));
    } else {
        perror("Invalid algorithm " + algorithm);
        exit(EXIT_FAILURE);
    }
}

/**
 * This function saves the densities obtained by the Wang-Landau algorithm.
 *
 * @param density, which must be saved.
 * @param directory where the result should be saved.
 * @param numberOfVertices of the graph.
 * @param lowerBound, the minimum value for accepting the step.
 * @param upperBound, the maximum value for accepting the step.
 * @param numberOfCounts, how often has a number been added to the density.
 */
void MCSimulation::saveDensity(double density[], const std::string directory,
                                int numberOfVertices, int lowerBound, int upperBound, unsigned long long int numberOfCounts) {
    double p = 0.0;
    double err = 0.0;
    int i = 0;
    double sum = 0.0;
    std::ofstream outfile;
    outfile.open((directory + "/" + "density_" + numToStr(numberOfVertices) + "_"
                                               + numToStr(lowerBound) + "_"
                                               + numToStr(upperBound) + ".dat").c_str());
    // sum over the density and sum it up
    for (i = (lowerBound - 1); i < (upperBound); i++) {
        sum += density[i];
    }
    // print the density plus the error
    for (i = (lowerBound - 1); i < (upperBound); i++) {
        if (density[i] != 0) {
            // just save, if density[i] is not zero
            p = density[i] / (sum);
            err = sqrt((p * (1.0 - p)) / ((double) numberOfCounts - 1.0));
            outfile << (i + 1) << " " << density[i] << " " << err << std::endl;
        }
    }
    outfile.close();
}
