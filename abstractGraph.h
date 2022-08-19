/*
 * abstractGraph.h
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

#ifndef ABSTRACTGRAPH_H_INCLUDED
#define ABSTRACTGRAPH_H_INCLUDED

#include <fstream>
#include <stack>

#include "edge.h"
#include "previousState.h"
#include "randomGenerator.h"

/**
 * Define an abstract graph representation.
 * This class can handle both directed- and undirected graphs.
 * Furthermore loops can be either enabled of disabled.
 */
class AbstractGraph {
public:
    /**
     * Constructor for setting up an empty AbstractGraph.
     *
     * @param n number of vertices.
     * @param generator a random number generator.
     * @param digraph true (default): digraph, false: no digraph.
     * @param loopsAllowed true (default): loops enabled, false: loops disabled.
     */
    AbstractGraph(int n, RandomGenerator* generator,
                  bool digraph = false, bool loopsAllowed = false);

    /**
     * Destructor.
     */
    virtual ~AbstractGraph();

    /**
     * Set all elements of the adjacency matrix to false.
     */
    void resetGraph();

    /**
     * Save the graph as graphviz file.
     *
     * @param directory where the file should be saved.
     * @param fileName name of the graphviz file to save.
     */
    void saveGraphToGraphviz(const std::string directory, const std::string fileName);

    /**
     * Generate a fully connected graph with exactly one component
     * (size of the largest component = numberOfVertices).
     * This means all non diagonal elements of the adjacency matrix have to be set true.
     */
    void generateCompleteGraph();

    /**
     * Align all vertices on one line (size of the largest component = numberOfVertices).
     */
    void generateLine();

    /**
     * Generate a graph with randomly set edges.
     */
    void generateRandom();

    /**
     * Generate a candidate graph, where one vertex is choosen randomly. After removing
     * all edges of this vertex, one inserts all feasible edges with a certain probability.
     *
     * @param vertex, pointer to a vertex, which will contain the choosen vertex after
     *       calling this function.
     *
     * @return a backup of the current state.
     */
    previousState generateCandidateGraph(int* vertex);

    /**
     * This function returns the size of the largest component. Maybe the size must be
     * recalculated.
     *
     * @return the size of the largest component.
     */
    int getSizeOfLargestComponent();

    /**
     * This function returns the number of components.
     * Maybe getSizeOfLargestComponent has to executed.
     *
     * @return the number of components.
     */
    int getNumberOfComponents();

    /**
     * Return the number of edges of the graph.
     *
     * @return the number of edges.
     */
    int getNumberOfEdges();

    /**
     * Return the number of vertices of the graph.
     *
     * @return the number of vertices.
     */
    int getNumberOfVertices();

    /**
     * Return the entry of the adjacency matrix for the given vertices.
     *
     * @param vertex1 the index of the first vertex.
     * @param vertex2 the index of the second vertex.
     *
     * @return the entry of the tuple (vertex1, vertex2).
     */
    int getAdjacencyMatrixEntry(int vertex1, int vertex2);

    /**
     * Return whether the graph is directed (true) or undirected (false).
     *
     * @return directed / undirected graph.
     */
    int getIsDigraph();

    /**
     * Return whether loops are enabled (true) or disabled (false).
     *
     * @return loops enabled / disabled.
     */
    int getLoopsEnabled();

    /**
     * Return the RandomGenerator.
     *
     * @return RandomGenerator.
     */
    RandomGenerator* getRandomGenerator();

    /**
     * Revert the step graph => candidate. Remove all edges of the given
     * vertex and add all old ones.
     *
     * @param vertex, the index of the given vertex.
     * @param safedState, backup of the previous state.
     */
    void revertGenerateCandidateGraph(int vertex, previousState safedState);

    /**
     * This function inserts one edge randomly into the graph.
     */
    void generateRandomEdge();

protected:
    bool **adjacencyMatrix;
    bool isDigraph;
    RandomGenerator *randomGenerator;

    /**
     * Should the edge (vertex1, vertex2) be inserted?
     * This function is abstract and must be implemented by inheriting classes.
     *
     * @param vertex1 index of the head of the proposed edge.
     * @param vertex2 index of the target of the propose edge.
     *
     * @return true: insert the proposed edge;
     *    false: do not insert the proposed edge.
     */
    virtual bool mustInsertEdge(int vertex1, int vertex2) = 0;

private:
    int numberOfVertices;
    int numberOfEdges;
    int largestComponentSize;
    int numberOfComponents;
    bool loopsEnabled;

    /**
     * Find an unmarked vertex, which is the tail of an edge from the given vertex.
     * This function is an auxiliary function for calculating the size of the largest components.
     *
     * @param vertex index of the given vertex.
     * @param marked an array of length numberOfVertices which contains informations
     *       about the visited vertices.
     *
     * @return the index of an unmarked vertex of -1, if all vertices are marked.
     */
    int unmarkedVertex(int vertex, bool marked[]);

    /**
     * Calculate the size of the largest component using a depth-first search algorithm.
     */
    void sizeOfLargestComponent();

    /**
     * This function inserts for the given index all feasible edges with
     * a certain probability.
     * Note: This function requires that all inheriting classes overwrite
     * function mustInsertEdge which decides for a vertex, if it is inserted.
     *
     * @param vertex the index of the given vertex.
     */
    void insertEdgesRandom(int vertex);

    /**
     * Delete all edges of the given vertex.
     *
     * @param vertex, the index of the given vertex.
     *
     * @return stack with all indices of removed edges.
     */
    std::stack<edge> removeAllEdgesOfVertex(int vertex);

    /**
     * Add the given edge to the graph, if possible.
     *
     * @param vertex1, index of the first vertex.
     * @param vertex2, index of the second vertex.
     */
    void addEdge(int vertex1, int vertex2);

    /**
     * Add the given edge to the digraph, if possible.
     *
     * @param vertex1 index of the first vertex.
     * @param vertex2 index of the second vertex.
     */
    void addEdgeDigraph(int vertex1, int vertex2);

    /**
     * Add the given edge to the undirected graph, if possible.
     *
     * @param vertex1 index of the first vertex.
     * @param vertex2 index of the second vertex.
     */
    void addEdgeNonDigraph(int vertex1, int vertex2);
};

#endif // ABSTRACTGRAPH_H_INCLUDED
