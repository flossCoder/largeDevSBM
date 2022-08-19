/*
 * mcPresenter.cpp
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

#include "mcPresenter.h"

/**
 * Constructor for setting up the presenter.
 *
 * @param p, parameter for setting up a graph.
 * @param d, where the file should be saved.
 * @param n, number of vertices.
 * @param seed, the seed for the random number generator.
 * @param generatorNumber, the generator to use, default is 0 == RandomGenerator().
 * @param graphNumber, the graph to use, default is 0 == ER random graph.
 * @param valueNumber, the value to use for the algorithm, default is 0 == largest component.
 */
MCPresenter::MCPresenter(GraphParameter *p, std::string d, int n, unsigned short int seed,
                         int generatorNumber, int graphNumber, int valueNumber){
    parameter = p;
    whichGenerator = generatorNumber;
    whichGraph = graphNumber;
    whichValue = valueNumber;
    numberOfVertices = n;
    generator = MCPresenter::initRandomGenerator(seed);
    simulation = new MCSimulation(d, this);
}

/**
 * Destructor.
 */
MCPresenter::~MCPresenter() {
    delete generator;
    delete simulation;
}

/**
 * Get the value of interest from the graph.
 *
 * @param graph, pointer to the given graph.
 *
 * @return The value of interest for the given graph.
 *
 * @exception invalid value in whichValue.
 */
double MCPresenter::getValueOfInterest(AbstractGraph *graph) {
    if (whichValue == GETLARGESTCOMPONENTSIZE) {
        return (graph->getSizeOfLargestComponent());
    } else {
        perror("Invalid whichValue " + whichValue);
        exit(EXIT_FAILURE);
    }
}

/**
 * Provide some further statistics.
 *
 * @param outfile, reference to the outfile-object to save the statistics.
 * @param graph, pointer to the given graph.
 *
 * @exception invalid value in whichValue.
 */
void MCPresenter::printStatistics(std::ofstream& outfile, AbstractGraph *graph) {
    if (whichValue == GETLARGESTCOMPONENTSIZE) {
        outfile << graph->getNumberOfComponents() << " " << graph->getNumberOfEdges();
    } else {
        perror("Invalid whichValue " + whichValue);
        exit(EXIT_FAILURE);
    }
}

/**
 * Generate a new graph.
 *
 * @param startCondition, 0: generate a complete graph
 *                        1: generate line graph
 *                        2: generate random graph
 *                        3: generate empty graph
 *
 * @return pointer to the generated graph.
 *
 * @exception invalid whichGraph.
 * @exception invalid starting condition.
 */
AbstractGraph *MCPresenter::generateGraph(int startCondition) {
    AbstractGraph *graph;

    // set graph type depending on whichGraph
    if (whichGraph == GENERATEER) {
        // ER graphs
        graph = new ER(parameter->connectivity,
                       numberOfVertices,
                       MCPresenter::generator,
                       parameter->isDigraph,
                       parameter->loopsAllowed);
    } else if (whichGraph == GENERATESBM) {
        // SBM
        graph = new SBM(parameter->connectivity,
                        parameter->secondConnectivity,
                        parameter->numberOfBlocks,
                        numberOfVertices,
                        MCPresenter::generator,
                        parameter->isDigraph,
                        parameter->loopsAllowed);
    } else {
        perror("Invalid whichGraph " + whichGraph);
        exit(EXIT_FAILURE);
    }

    // set the required startCondition
    if (startCondition == GENERATECOMPLETEGRAPH) {
        graph->generateCompleteGraph();
    } else if (startCondition == GENERATELINEGRAPH) {
        graph->generateLine();
    } else if (startCondition == GENERATERANDOMGRAPH) {
        graph->generateRandom();
    } else if (startCondition == GENERATEEMPTYGRAPH) {
        // no action required, because the initially generated graph is empty
    } else {
        perror("Invalid startCondition " + startCondition);
        exit(EXIT_FAILURE);
    }

    // return the pointer to the graph-object
    return(graph);
}

/**
 * Initialize a new random number generator.
 *
 * @param seed, the seed for the random number generator.
 *
 * @return pointer to the random number generator.
 *
 * @exception invalid value in whichGenerator.
 */
RandomGenerator *MCPresenter::initRandomGenerator(short unsigned int seed) {
    RandomGenerator *gen;
    if (whichGenerator == DEFAULTGENERATOR) {
        gen = new RandomGenerator(seed);
    } else {
        perror("Invalid whichGenerator " + whichGenerator);
        exit(EXIT_FAILURE);
    }
    return(gen);
}

/**
 * Generate a set of graphs and calculate a histogram.
 *
 * @param numberOfGraphs, how many graphs should be generated randomly.
 */
void MCPresenter::simpleSampling(int numberOfGraphs) {
    simulation->simpleSampling(numberOfGraphs);
}

/**
 * Perform the simulation on one graph under one starting condition
 * using the Metropolis algorithm.
 *
 * @param temperature, the artificial temperature, make sure temperature != 0.
 * @param sweeps, how many sweeps should be recorded.
 * @param equilibrationSweeps, number of steps to perform, until the simulation is equilibrated.
 */
void MCPresenter::metropolisAlgorithm(double temperature, int sweeps, int equilibrationSweeps) {
    simulation->metropolisAlgorithm(temperature, sweeps, equilibrationSweeps);
}

/**
 * Calculate the number of steps required to equilibrate the simulation
 *
 * @param temperature, the artificial temperature, make sure temperature != 0.
 * @param equilibrationSweeps, the number of sweeps to perform for this calculation.
 */
void MCPresenter::calculateEquilibrationSteps(double temperature, int equilibrationSweeps) {
    simulation->calculateEquilibrationSteps(temperature, equilibrationSweeps);
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
void MCPresenter::wangLandauAlgorithm(int lowerBound, int upperBound, int sweepsToEvaluate, double modiFacFinal) {
    simulation->wangLandauAlgorithm(lowerBound, upperBound, sweepsToEvaluate, modiFacFinal);
}

/**
 * Getter for the number of vertices.
 *
 * @return the number of vertices.
 */
int MCPresenter::getNumberOfVertices() {
    return(numberOfVertices);
}

/**
 * Return a random number in [0, 1].
 *
 * @return The random number.
 */
double MCPresenter::randomNumber() {
    return (generator->randomNumber());
}
