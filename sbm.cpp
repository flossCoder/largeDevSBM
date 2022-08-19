/*
 * sbm.cpp
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

#include "sbm.h"

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
SBM::SBM(double interConnectivity, double intraConnectivity, int blocks,
int n, RandomGenerator* generator, bool digraph, bool loopsAllowed)
: AbstractGraph(n, generator, digraph, loopsAllowed) {
    interBlockConnectivity = interConnectivity;
    intraBlockConnectivity = intraConnectivity;
    numberOfBlocks = blocks;
    interProbability = interConnectivity / n;
    intraProbability = intraConnectivity / n;
    labeling = new int[n];
    blockCounter = new int[blocks];
    int i, blockIndex;
    // initialize labeling
    for (i = 0; i < n; i++) {
        labeling[i] = 0;
    }
    // initialize blockCounter
    for (i = 0; i < blocks; i++) {
        blockCounter[i] = 0;
    }
    // add all vertices randomly to one block
    for (i = 0; i < n; i++) {
        blockIndex = blocks * randomGenerator->randomNumber();
        //std::cout << blockIndex << std::endl;
        labeling[i] = blockIndex;
        blockCounter[blockIndex] = blockCounter[blockIndex] + 1;
    }
}

/**
 * Destructor.
 */
SBM::~SBM() {
    delete [] labeling;
    delete [] blockCounter;
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
bool SBM::mustInsertEdge(int vertex1, int vertex2) {
    if (labeling[vertex1] == labeling[vertex2]) {
        // case: both vertices are in the same block
        return (randomGenerator->randomNumber() <= intraProbability);
    } else {
        // case: both vertices are in different blocks
        return (randomGenerator->randomNumber() <= interProbability);
    }
}

/******************* save results *******************/

/**
 * Save the graph as graphviz file.
 *
 * @param directory where the file should be saved.
 * @param fileName name of the graphviz file to save.
 */
void SBM::saveGraphToGraphviz(const std::string directory, const std::string fileName) {
    // open file
    std::ofstream outfile;
    outfile.open((directory + "/" + fileName + ".gv").c_str());
    // write head of graphviz representation to file
    if (isDigraph) {
        outfile << "digraph G {" << std::endl;
    } else {
        outfile << "graph G {" << std::endl;
    }
    // write all edges to file
    for (int i = 0; i < getNumberOfVertices(); i++) {
        for (int j = 0; j < getNumberOfVertices(); j++) {
            if (adjacencyMatrix[i][j]) {
                if (isDigraph) {
                    outfile << i << " -> " << j << std::endl;
                } else {
                    outfile << i << " -- " << j << std::endl;
                }
            }
        }
    }

    for (int i = 0; i < getNumberOfVertices(); i++) {
        if (labeling[i] == 0) {
            outfile << i << " [style=filled, fillcolor=red]" << std::endl;
        }
        if (labeling[i] == 1) {
            outfile << i << " [style=filled, fillcolor=blue]" << std::endl;
        }
    }
    // write tail of graphviz representation to file
    outfile << "}" << std::endl;
    // close the file
    outfile.close();
}
