/*
 * mcSimulation.h
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

#ifndef MCSIMULATION_H_INCLUDED
#define MCSIMULATION_H_INCLUDED

#include <fstream>
#include <math.h>

#include "abstractGraph.h"
#include "edge.h"
#include "graphGeneratorMakros.h"
#include "histogram.h"
#include "mcPresenter.h"
#include "previousState.h"
#include "strNumConv.h"

class MCPresenter;

/**
 * Define some useful makros.
 */
#define USEMETROPOLISALGORITHM 0
#define USEWANGLANDAUALGORITHM 1

/**
 * Define a structure for saving some parameters.
 */
typedef struct {
    double temperature;
    double *density;
    double lowerBound;
    double upperBound;
} parameterCollector_type;

/**
 * This class defines a Monte-Carlo Simulation for simulating graphs.
 */
class MCSimulation {

public:
    /**
     * Constructor for setting up the simulation.
     *
     * @param d, where the file should be saved.
     * @param presenter, the preseter of the simulation.
     */
     MCSimulation(std::string d, MCPresenter *presenter);

    /**
     * Destructor.
     */
	~MCSimulation();

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
     *       histogram check is performed.
     * @param modiFacFinal, the final modification factor.
     */
    void wangLandauAlgorithm(int lowerBound, int upperBound, int sweepsToEvaluate, double modiFacFinal);

private:
    std::string directory;
    MCPresenter *simulationPresenter;
    Histogram *histogram;

    /**
     * Generate the next state for the simulation. This function generates the candidate graph,
     * calculates the size of the largest components for both graphs and sets the next state.
     *
     * @param algorithm, the used algorithm for importance sampling.
     * @param currentState, pointer to the current state of the graph.
     * @param parameters, the required parameters for the accept / reject decision.
     */
    void generateNextState(int algorithm, AbstractGraph *currentState, parameterCollector_type parameters);

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
    bool acceptNewState(int algorithm, double currentValue, double candidateValue,
                        parameterCollector_type parameters);

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
    void saveDensity(double density[], const std::string directory, int numberOfVertices,
                      int lowerBound, int upperBound, unsigned long long int numberOfCounts);
};

#endif // MCSIMULATION_H_INCLUDED
