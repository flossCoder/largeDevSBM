/*
 * sbm.h
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

#ifndef SBM_H_INCLUDED
#define SBM_H_INCLUDED

#include "abstractGraph.h"

/**
 * Implementation of the stochastic blockmodel.
 */
class SBM : public AbstractGraph {
public:
    /**
     * Constructor for setting up an empty SBM graph.
     *
     * @param interConnectivity inter block connectivity of the graph
     * @param intraConnectivity intra block connectivity of the graph
     * @param blocks the number of blocks
     * @param n number of vertices
     * @param generator a random number generator
     * @param digraph true (default): digraph, false: no digraph
     * @param loopsAllowed true (default): loops enabled, false: loops disabled
     */
    SBM(double interConnectivity, double intraConnectivity, int blocks,
        int n, RandomGenerator* generator,
        bool digraph = false, bool loopsAllowed = false);

    /**
     * Destructor.
     */
    ~SBM();

    /**
     * Should the edge (vertex1, vertex2) be inserted?
     * This function is abstract and must be implemented by inheriting classes.
     *
     * @param vertex1 index of the head of the proposed edge
     * @param vertex2 index of the target of the propose edge
     *
     * @return true: insert the proposed edge;
     *    false: do not insert the proposed edge
     */
    virtual bool mustInsertEdge(int vertex1, int vertex2);

    /**
     * Save the graph as graphviz file.
     *
     * @param directory where the file should be saved.
     * @param fileName name of the graphviz file to save.
     */
    void saveGraphToGraphviz(const std::string directory, const std::string fileName);

private:
    double interBlockConnectivity;
    double intraBlockConnectivity;
    double interProbability;
    double intraProbability;
    int numberOfBlocks;
    int *labeling;
    int *blockCounter;
};

#endif // SBM_H_INCLUDED
