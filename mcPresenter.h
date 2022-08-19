/*
 * mcPresenter.h
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

#ifndef MCPRESENTER_H_INCLUDED
#define MCPRESENTER_H_INCLUDED

#include <fstream>

#include "abstractGraph.h"
#include "er.h"
#include "graphGeneratorMakros.h"
#include "graphParameter.h"
#include "mcSimulation.h"
#include "randomGenerator.h"
#include "sbm.h"

class MCSimulation;

/**
 * Define some usefull makros.
 */
#define GETLARGESTCOMPONENTSIZE 0
#define GENERATEER 0
#define GENERATESBM 1
#define DEFAULTGENERATOR 0

/**
 * This class defines the presenter according to the MVP-pattern for
 * the Monte-Carlo Simulation for simulating graphs.
 */
class MCPresenter {
public:
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
    MCPresenter(GraphParameter *p, std::string d, int n, unsigned short int seed,
                 int generatorNumber = 0, int graphNumber = 0, int valueNumber = 0);

    /**
     * Destructor.
     */
	~MCPresenter();

	/**
     * Generate a set of graphs and calculate a histogram.
     *
     * @param numberOfGraphs, how many graphs should be generated randomly.
     */
    void simpleSampling(int numberOfGraphs);

    /**
     * Perform the simulation on one graph under one starting condition
     * using the Metropolis algorithm.
     *
     * @param temperature, the artificial temperature, make sure temperature != 0.
     * @param sweeps, how many sweeps should be recorded.
     * @param equilibrationSweeps, number of steps to perform, until the simulation is equilibrated.
     */
    void metropolisAlgorithm(double temperature, int sweeps, int equilibrationSweeps);

    /**
     * Calculate the number of steps required to equilibrate the simulation.
     *
     * @param temperature, the artificial temperature, make sure temperature != 0.
     * @param equilibrationSweeps, the number of sweeps to perform for this calculation.
     */
    void calculateEquilibrationSteps(double temperature, int equilibrationSweeps = 1000);

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
    void wangLandauAlgorithm(int lowerBound, int upperBound, int sweepsToEvaluate, double modiFacFinal);

    /**
     * Get the value of interest from the graph.
     *
     * @param graph, pointer to the given graph.
     *
     * @return The value of interest for the given graph.
     *
     * @exception invalid value in whichValue.
     */
    double getValueOfInterest(AbstractGraph *graph);

    /**
     * Provide some further statistics.
     *
     * @param outfile, reference to the outfile-object to save the statistics.
     * @param graph, pointer to the given graph.
     *
     * @exception invalid value in whichValue.
     */
    void printStatistics(std::ofstream& outfile, AbstractGraph *graph);

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
    AbstractGraph *generateGraph(int startCondition);

    /**
     * Initialize a new random number generator.
     *
     * @param seed, the seed for the random number generator.
     *
     * @return pointer to the random number generator.
     *
     * @exception invalid value in whichGenerator.
     */
    RandomGenerator *initRandomGenerator(short unsigned int seed);

    /**
     * Return a random number in [0, 1].
     *
     * @return The random number.
     */
     double randomNumber();

     /**
      * Getter for the number of vertices.
      *
      * @return the number of vertices.
      */
     int getNumberOfVertices();

private:
    GraphParameter *parameter;
    int whichGraph;
    int numberOfVertices;
    int whichGenerator;
    int whichValue;
    RandomGenerator *generator;
    MCSimulation *simulation;
};

#endif // MCPRESENTER_H_INCLUDED
