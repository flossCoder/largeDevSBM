/*
 * randomGenerator.h
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

#ifndef RANDOMGENERATOR_H_INCLUDED
#define RANDOMGENERATOR_H_INCLUDED

#include <stdlib.h>

/**
 * Define a class for generating random vertex indices.
 * Easily exchanging the random generator should be possible.
 */
class RandomGenerator {
public:
    /**
     * Constructor for the RandomGenerator.
     *
     * @param seed (starting value) for the random number generator
     */
    RandomGenerator(short unsigned int seed);

    /**
     * Randomly draw a vertex index.
     *
     * @param numberOfVertices the number of vertices
     *
     * @return the randomly choosen vertex index
     */
    int randomVertex(int numberOfVertices);

    /**
     * Return a random number in [0, 1].
     *
     * @return The random number.
     */
     double randomNumber();
};

#endif // RANDOMGENERATOR_H_INCLUDED
