/*
 * er.h
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

#ifndef ER_H_INCLUDED
#define ER_H_INCLUDED

#include "abstractGraph.h" // Base class: AbstractGraph

/**
 * Define an Erdos-Renyi random graph based on AbstractGraph.
 */
class ER : public AbstractGraph {

public:
	/**
     * Constructor for setting up an empty ER graph.
     *
     * @param c connectivity of the graph
     * @param n number of vertices
     * @param generator a random number generator
     * @param digraph true (default): digraph, false: no digraph
     * @param loopsAllowed true (default): loops enabled, false: loops disabled
     */
    ER(double c, int n, RandomGenerator* generator,
        bool digraph = false, bool loopsAllowed = false);

    /**
     * Destructor.
     */
    ~ER();

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
     * This function returns the connectivity of the graph.
     *
     * @return the connectivity of the graph
     */
    inline double getConnectivity()
    {
        return(connectivity);
    }

    /**
     * This function returns the probability for an edge to be present.
     *
     * @return the probability for an edge to be present
     */
     inline double getProbability()
    {
        return(probability);
    }

private:
    double connectivity;
    double probability;
};

#endif // ER_H_INCLUDED
