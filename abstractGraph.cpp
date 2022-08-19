/*
 * abstractGraph.cpp
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

#include "abstractGraph.h"

/******************* Constructors *******************/

/**
 * Constructor for setting up an empty AbstractGraph.
 *
 * @param n number of vertices.
 * @param generator a random number generator.
 * @param digraph true (default): digraph, false: no digraph.
 * @param loopsAllowed true (default): loops enabled, false: loops disabled.
 */
AbstractGraph::AbstractGraph(int n, RandomGenerator* generator, bool digraph, bool loopsAllowed) {
    randomGenerator = generator;
    numberOfVertices = n;
    numberOfEdges = 0; // initially the graph has no edges
     // Per convention is the size of the largest component -1, as long it has not been calculated.
    largestComponentSize = -1;
    numberOfComponents = -1;
    adjacencyMatrix = new bool*[n];
    for (int i = 0; i < n; i++) {
        adjacencyMatrix[i] = new bool[n];
    }
    AbstractGraph::resetGraph(); // initially all fields of the adjacency matrix should be zero
    isDigraph = digraph;
    loopsEnabled = loopsAllowed;
}

/******************* Destructor *******************/

/**
 * Destructor.
 */
AbstractGraph::~AbstractGraph() {
     for (int i = 0; i < numberOfVertices; i++) {
         delete [] adjacencyMatrix[i];
     }
     delete [] adjacencyMatrix;
}

/******************* getter *******************/

/**
 * Return the number of edges of the graph.
 *
 * @return the number of edges.
 */
int AbstractGraph::getNumberOfEdges() {
    return(numberOfEdges);
}

/**
 * Return the number of vertices of the graph.
 *
 * @return the number of vertices.
 */
int AbstractGraph::getNumberOfVertices() {
    return(numberOfVertices);
}

/**
 * Return the entry of the adjacency matrix for the given vertices.
 *
 * @param vertex1 the index of the first vertex.
 * @param vertex2 the index of the second vertex.
 *
 * @return the entry of the tuple (vertex1, vertex2).
 */
int AbstractGraph::getAdjacencyMatrixEntry(int vertex1, int vertex2) {
    return(adjacencyMatrix[vertex1][vertex2]);
}

/**
 * Return whether the graph is directed (true) or undirected (false).
 *
 * @return directed / undirected graph.
 */
int AbstractGraph::getIsDigraph() {
    return(isDigraph);
}

/**
 * Return whether loops are enabled (true) or disabled (false).
 *
 * @return loops enabled / disabled.
 */
int AbstractGraph::getLoopsEnabled() {
    return(loopsEnabled);
}

/**
 * Return the RandomGenerator.
 *
 * @return RandomGenerator.
 */
RandomGenerator* AbstractGraph::getRandomGenerator() {
    return(randomGenerator);
}

/**
 * This function returns the size of the largest component. Maybe the size must be
 * recalculated.
 *
 * @return the size of the largest component.
 */
int AbstractGraph::getSizeOfLargestComponent() {
    if (largestComponentSize == -1) {
        // recalculate the size of the largest component
        AbstractGraph::sizeOfLargestComponent();
    }
    return(largestComponentSize);
}

/**
 * This function returns the number of components.
 * Maybe getSizeOfLargestComponent has to executed.
 *
 * @return the number of components.
 */
int AbstractGraph::getNumberOfComponents() {
    if (largestComponentSize == -1) {
        // recalculate the size of the largest component
        AbstractGraph::sizeOfLargestComponent();
    }
    return(numberOfComponents);
}

/******************* graph generators *******************/

/**
 * Generate a fully connected graph with exactly one component
 * (size of the largest component = numberOfVertices).
 * This means all non diagonal elements of the adjacency matrix have to be set true.
 */
void AbstractGraph::generateCompleteGraph() {
    numberOfEdges = 0;
    largestComponentSize = numberOfVertices;
    numberOfComponents = 1;
    if (isDigraph) {
        // treat case of an directed graph
        for (int i = 0; i < numberOfVertices; i++) {
            for (int j = 0; j < numberOfVertices; j++) {
                if (i != j) {
                    adjacencyMatrix[i][j] = true;
                    numberOfEdges++;
                } else {
                    adjacencyMatrix[i][j] = false;
                }
            }
        }
    } else {
        // treat case of an undirected graph
        for (int i = 0; i < numberOfVertices; i++) {
            // just iterate over all i + 1 elements, because edge (a, b) = (b, a)
            for (int j = i + 1; j < numberOfVertices; j++) {
                adjacencyMatrix[j][i] = true;
                numberOfEdges++;
            }
        }
    }
}

/**
 * Align all vertices on one line (size of the largest component = numberOfVertices).
 */
void AbstractGraph::generateLine() {
    // here no decision between directed- and undirected graphs is required, because all
    // vertices will be in the lower triangular matrix
	if (numberOfEdges != 0) {
        AbstractGraph::resetGraph();
    }
    // insert an edge from vertex i to vertex i + 1
    for (int i = 0; i < (numberOfVertices - 1); i++) {
        adjacencyMatrix[(i + 1)][i] = true;
    }
    numberOfEdges = numberOfVertices;
    largestComponentSize = numberOfVertices;
    numberOfComponents = 1;
}

/**
 * Generate a graph with randomly setted edges.
 */
void AbstractGraph::generateRandom() {
    if (numberOfEdges != 0) {
        AbstractGraph::resetGraph();
    }
    if (isDigraph) {
        for (int i = 0; i < numberOfVertices; i++) {
            for (int j = 0; j < numberOfVertices; j++) {
                if (mustInsertEdge(i, j)) {
                    AbstractGraph::addEdgeDigraph(i, j);
                }
            }
        }
    } else {
        for (int i = 0; i < numberOfVertices; i++) {
            // just iterate over all i + 1 elements, because edge (a, b) = (b, a)
            for (int j = i; j < numberOfVertices; j++) {
                if (mustInsertEdge(i, j)) {
                    AbstractGraph::addEdgeNonDigraph(i, j);
                }
            }
        }
    }
}

/**
 * Generate a candidate graph, where one vertex is choosen randomly. After removing
 * all edges of this vertex, one inserts all feasible edges with a certain probability.
 *
 * @param vertex, pointer to a vertex, which will contain the choosen vertex after
 *       calling this function.
 *
 * @return a backup of the current state.
 */
previousState AbstractGraph::generateCandidateGraph(int *vertex) {
    // 1. step: choose vertex X randomly
    *vertex = randomGenerator->randomVertex(numberOfVertices);
    // 2. step: generate backup
    previousState safedState;
    safedState.numberOfComponents = numberOfComponents;
    safedState.numberOfEdges = numberOfEdges;
    safedState.sizeLargestComp = largestComponentSize;
    // 3. step: delete all edges of X
    safedState.edgesStack = AbstractGraph::removeAllEdgesOfVertex(*vertex);
    // 4. step: insert all feasible edges (with probability)
    AbstractGraph::insertEdgesRandom(*vertex);

    return(safedState);
}

/**
 * Revert the step graph => candidate. Remove all edges of the given
 * vertex and add all old ones.
 *
 * @param vertex, the index of the given vertex.
 * @param safedState, backup of the previous state.
 */
void AbstractGraph::revertGenerateCandidateGraph(int vertex, previousState safedState) {
    // 1. remove all edges of the given vertex
    AbstractGraph::removeAllEdgesOfVertex(vertex);
    // 2. add all edges from the stack
    while (!safedState.edgesStack.empty()) {
        AbstractGraph::addEdge(safedState.edgesStack.top().head, safedState.edgesStack.top().tail);
        safedState.edgesStack.pop();
    }
    // 3. reset properties of the state
    AbstractGraph::largestComponentSize = safedState.sizeLargestComp;
    AbstractGraph::numberOfComponents = safedState.numberOfComponents;
    AbstractGraph::numberOfEdges = safedState.numberOfEdges;
}

/**
 * Set all elements of the adjacency matrix to false.
 */
void AbstractGraph::resetGraph() {
    numberOfEdges = 0;
    largestComponentSize = -1;
    numberOfComponents = -1;
    for (int i = 0; i < numberOfVertices; i++) {
        for (int j = 0; j < numberOfVertices; j++) {
            adjacencyMatrix[i][j] = false;
        }
    }
}

/******************* change edges *******************/

/**
 * This function inserts one edge randomly into the graph.
 */
void AbstractGraph::generateRandomEdge() {
    int vertex1 = randomGenerator->randomVertex(numberOfVertices);
    int vertex2 = randomGenerator->randomVertex(numberOfVertices);
    int edges = numberOfEdges;
    AbstractGraph::addEdge(vertex1, vertex2);
    // retry until succeeded, if necessary
    while (edges == numberOfEdges) {
        vertex1 = randomGenerator->randomVertex(numberOfVertices);
        vertex2 = randomGenerator->randomVertex(numberOfVertices);
        AbstractGraph::addEdge(vertex1, vertex2);
    }
}

/**
 * This function inserts for the given index all feasible edges with
 * a certain probability.
 * Note: This function requires that all inheriting classes overwrite
 * function mustInsertEdge which decides for a vertex, if it is inserted.
 *
 * @param vertex the index of the given vertex.
 */
void AbstractGraph::insertEdgesRandom(int vertex) {
    if (isDigraph) {
        for (int i = 0; i < numberOfVertices; i++) {
            // insert outgoing edge
            if (mustInsertEdge(vertex, i)) {
                AbstractGraph::addEdgeDigraph(vertex, i);
            }
            // insert ingoing edge
            if (mustInsertEdge(i, vertex)) {
                AbstractGraph::addEdgeDigraph(i, vertex);
            }
        }
    } else {
        for (int i = 0; i < numberOfVertices; i++) {
            // just one insertion is required
            if (mustInsertEdge(vertex, i)) {
                AbstractGraph::addEdgeNonDigraph(vertex, i);
            }
        }
    }
}

/**
 * Delete all edges of the given vertex.
 *
 * @param vertex the index of the given vertex.
 *
 * @return stack with all indices of removed edges.
 */
std::stack<edge> AbstractGraph::removeAllEdgesOfVertex(int vertex) {
    std::stack<edge> stack;
    largestComponentSize = -1;
    int i;
    for (i = 0; i < numberOfVertices; i++) {
        if (adjacencyMatrix[vertex][i]) {
            edge a;
            a.head = vertex;
            a.tail = i;
            stack.push(a);
            adjacencyMatrix[vertex][i] = false;
            numberOfEdges--;
        }
        if (adjacencyMatrix[i][vertex]) {
            edge a;
            a.head = i;
            a.tail = vertex;
            stack.push(a);
            adjacencyMatrix[i][vertex] = false;
            numberOfEdges--;
        }
    }
    return(stack);
}

/**
 * Add the given edge to the graph, if possible.
 *
 * @param vertex1 index of the first vertex.
 * @param vertex2 index of the second vertex.
 */
void AbstractGraph::addEdge(int vertex1, int vertex2) {
    if (isDigraph) {
        AbstractGraph::addEdgeDigraph(vertex1, vertex2);
    } else {
        AbstractGraph::addEdgeNonDigraph(vertex1, vertex2);
    }
}

/**
 * Add the given edge to the digraph, if possible.
 *
 * @param vertex1 index of the first vertex.
 * @param vertex2 index of the second vertex.
*/
void AbstractGraph::addEdgeDigraph(int vertex1, int vertex2) {
    largestComponentSize = -1;
    if ((!adjacencyMatrix[vertex1][vertex2]) && ((!loopsEnabled && (vertex1 != vertex2)) || loopsEnabled)) {
        adjacencyMatrix[vertex1][vertex2] = true;
        numberOfEdges++;
    }
}

/**
 * Add the given edge to the undirected graph, if possible.
 *
 * @param vertex1 index of the first vertex.
 * @param vertex2 index of the second vertex.
 */
void AbstractGraph::addEdgeNonDigraph(int vertex1, int vertex2) {
    largestComponentSize = -1;
    // in case of an non digraph, where the edge has no direction, it is just inserted once
    if ((vertex1 > vertex2) && !adjacencyMatrix[vertex1][vertex2]) {
        // convention: first index is bigger than second
        adjacencyMatrix[vertex1][vertex2] = true;
        numberOfEdges++;
    } else if ((vertex1 < vertex2) && !adjacencyMatrix[vertex2][vertex1]) {
        // convention: first index is bigger than second
        adjacencyMatrix[vertex2][vertex1] = true;
        numberOfEdges++;
    } else if ((vertex1 == vertex2) && !adjacencyMatrix[vertex1][vertex1] && loopsEnabled) {
        adjacencyMatrix[vertex1][vertex1] = true;
        numberOfEdges++;
    }
}

/******************* save results *******************/

/**
 * Save the graph as graphviz file.
 *
 * @param directory where the file should be saved.
 * @param fileName name of the graphviz file to save.
 */
void AbstractGraph::saveGraphToGraphviz(const std::string directory, const std::string fileName) {
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
    for (int i = 0; i < numberOfVertices; i++) {
        for (int j = 0; j < numberOfVertices; j++) {
            if (adjacencyMatrix[i][j]) {
                if (isDigraph) {
                    outfile << i << " -> " << j << std::endl;
                } else {
                    outfile << i << " -- " << j << std::endl;
                }
            }
        }
    }
    // write tail of graphviz representation to file
    outfile << "}" << std::endl;
    // close the file
    outfile.close();
}

/******************* calculation of the size of largest components *******************/

/**
 * Calculate the size of the largest component using a depth-first search algorithm.
 */
void AbstractGraph::sizeOfLargestComponent() {
    // declare all variables
    std::stack<int> stack;
    int counter, startVertex, current, numberOfMarkedVertices;
    bool marked[numberOfVertices];
    // initialize variables
    numberOfMarkedVertices = 0;
    startVertex = 0;
    for (int i = 0; i < numberOfVertices; i++) {
        marked[i] = false;
    }

    numberOfComponents = 0;

    while (numberOfMarkedVertices < numberOfVertices) {
        // find current start vertex
        while ((numberOfMarkedVertices < numberOfVertices) && marked[startVertex]) {
            startVertex++;
        }
        if (startVertex == numberOfVertices) {
            // case, when all components have been found
            return;
        }
        // found a component, that has not been treated jet
        current = startVertex;
        counter = 1;
        numberOfComponents++;
        stack.push(current);
        marked[current] = true;
        numberOfMarkedVertices++;
        while (!stack.empty()) {
            current = unmarkedVertex(current, marked);
            // current contains an index to an unmarked vertex or -1,
            // if no such vertex exists
            if (current != -1) {
                // found an unmarked vertex
                marked[current] = true;
                numberOfMarkedVertices++;
                stack.push(current);
                counter++;
            } else {
                if (counter > largestComponentSize) {
                    largestComponentSize = counter;
                }
                stack.pop();
                if (!stack.empty()) {
                    current = stack.top();
                }
            }
        }
    }
}

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
int AbstractGraph::unmarkedVertex(int vertex, bool marked[]) {
    for (int j = 0; j < numberOfVertices; j++) {
        if (!marked[j]) {
            if (isDigraph) {
                // directed graph
                if (adjacencyMatrix[vertex][j]) {
                    return(j);
                }
            } else {
                // undirected graph
                if (vertex > j) {
                    // convention: first index is bigger than second
                    if (adjacencyMatrix[vertex][j]) {
                        return(j);
                    }
                } else if (j > vertex) {
                    // convention: first index is bigger than second
                    if (adjacencyMatrix[j][vertex]) {
                        return(j);
                    }
                }
            }
        }
    }
    return(-1);
}
