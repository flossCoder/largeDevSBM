/*
 * er.cpp
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

#include "er.h"

/**
 * Constructor for setting up an empty ER graph.
 *
 * @param c connectivity of the graph
 * @param n number of vertices
 * @param generator a random number generator
 * @param digraph true (default): digraph, false: no digraph
 * @param loopsAllowed true (default): loops enabled, false: loops disabled
 */
ER::ER(double c, int n, RandomGenerator* generator, bool digraph, bool loopsAllowed)
: AbstractGraph(n, generator, digraph, loopsAllowed) { // call super constructor
    connectivity = c;
    probability = c/((double) n);
}

/**
 * Destructor.
 */
ER::~ER() {
}

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
bool ER::mustInsertEdge(int vertex1, int vertex2) {
    if (randomGenerator->randomNumber() <= probability) {
        return(true);
    } else {
        return(false);
    }
}
