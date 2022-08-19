/*
 * histogram.h
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

#ifndef HISTOGRAM_H_INCLUDED
#define HISTOGRAM_H_INCLUDED

#include <fstream>
#include <math.h>

/**
 * Histogram defines a simple histogram container.
 */
class Histogram {
public:
    /**
     * Constructor for the historgram: Allocate memory and set the histogram range.
     *
     * @param maxSize, maximum size of the largest component.
     */
    Histogram(int maxSize);

    /**
     * Destructor: Free the histogram.
     */
    ~Histogram();

    /**
     * Increment the correct histogram bin.
     *
     * @param x, the given value which must be incremented.
     *
     * @exception invalid given value.
     */
    void increment(int x);

    /**
     * Save the histogram to a dat-text file.
     *
     * @param directory where the result should be saved.
     * @param fileName the name of the file to save.
     */
    void saveHistogram(const std::string directory, const std::string fileName);

    /**
     * Set all histogram entries plus numberOfCounts to zero.
     */
    void resetHistogram();

    /**
     * Check, whether the histogram is flat enough.
     * Definition of "flat enough":
     * All entries lay in the epsilon environment of the average height of the histogram.
     *
     * @param epsilon Define the environment around the average height.
     * @param lowerBound, the minimum value for accepting the step.
     * @param upperBound, the maximum value for accepting the step.
     *
     * @return True: the histogram is flat enough; false: otherwise
     */
    bool isHistogramFlatEnough(double epsilon, int lowerBound, int upperBound);

    /**
     * Test whether all bins of the histogram are non zero.
     *
     * @param lowerBound, the minimum value for accepting the step.
     * @param upperBound, the maximum value for accepting the step.
     *
     * @return True: All bins are non zero; false: There exist bin(s) that are zero.
     */
    bool areAllBinsNonZero(int lowerBound, int upperBound);

private:
    int *histogram;
    int sizeOfHistogram;
    int numberOfCounts;

    /**
     * Search for the height of the smallest bin of the histogram.
     *
     * @param lowerBound, the minimum value for accepting the step.
     * @param upperBound, the maximum value for accepting the step.
     *
     * @return the height of the smallest bin.
     */
    int minimumValue(int lowerBound, int upperBound);
};

#endif // HISTOGRAM_H_INCLUDED
