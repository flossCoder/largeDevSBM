/*
 * graphParameter.h
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

#ifndef GRAPHPARAMETER_H_INCLUDED
#define GRAPHPARAMETER_H_INCLUDED

/**
 * This class serves as a container for some parameter needed for
 * initializing a graph.
 */
class GraphParameter {
public:
    bool isDigraph;
    bool loopsAllowed;
    double connectivity;
    double secondConnectivity;
    int numberOfBlocks;

    /**
     * The default constructor makes sure, that all parameter have
     * been initialized to avoid strang behaviour.
     */
    inline GraphParameter() {
        isDigraph = false;
        loopsAllowed = false;
        connectivity = 0.0;
        secondConnectivity = 0.0;
        numberOfBlocks = 0;
    }
};

#endif // GRAPHPARAMETER_H_INCLUDED
