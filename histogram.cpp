/*
 * histogram.cpp
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

#include "histogram.h"

/**
 * Constructor for the historgram: Allocate memory and set the histogram range.
 *
 * @param maxSize, maximum size of the largest component.
 */
Histogram::Histogram(int maxSize) {
    sizeOfHistogram = maxSize;
    histogram = new int[sizeOfHistogram];
    Histogram::resetHistogram();
}

/**
 * Destructor: Free the histogram.
 */
Histogram::~Histogram() {
    delete [] histogram;
}

/**
 * Increment the correct histogram bin.
 *
 * @param x, the given value which must be incremented.
 *
 * @exception invalid given value.
 */
void Histogram::increment(int x) {
    if ((x < 1) || (x > sizeOfHistogram)) {
        perror("Invalid given value x = " + x);
    }
    histogram[x - 1] = histogram[x - 1] + 1;
    numberOfCounts++;
}

/**
 * Save the histogram to a dat-text file.
 *
 * @param directory where the result should be saved.
 * @param fileName the name of the file to save.
 */
void Histogram::saveHistogram(const std::string directory, const std::string fileName) {
    double p = 0.0;
    double err = 0.0;
    std::ofstream outfile;
    outfile.open((directory + "/" + fileName + ".dat").c_str());
    for (int i = 0; i < sizeOfHistogram; i++) {
        if (histogram[i] != 0) {
            // only save non zero entries
            // save:
                // bin number
                // histogram entry for bin number
                // error of bin: err = sqrt(var(y) / (N - 1)) = sqrt(p * (p - 1) / (N - 1)); with:
                    // p = binCounter / N
                    // N = number of counts
            p = ((double) histogram[i]) / ((double) numberOfCounts);
            err = sqrt(((double) p * (1.0 - (double) p)) / ((double) numberOfCounts - 1.0));
            outfile << (i + 1) << " " << histogram[i] << " " << err << std::endl;
        }
    }
    outfile.close();
}

/**
 * Set all histogram entries plus numberOfCounts to zero.
 */
void Histogram::resetHistogram() {
    for (int i = 0; i < sizeOfHistogram; i++) {
        histogram[i] = 0;
    }
    numberOfCounts = 0;
}

/**
 * Check, whether the histogram is flat enough.
 * Definition of "flat enough":
 * All entries lay in the epsilon environment of the average height of the histogram.
 *
 * @param epsilon, define the environment around the average height.
 * @param lowerBound, the minimum value for accepting the step.
 * @param upperBound, the maximum value for accepting the step.
 *
 * @return True: the histogram is flat enough; false: otherwise
 */
bool Histogram::isHistogramFlatEnough(double epsilon, int lowerBound, int upperBound) {
    // avg = ((double) numberOfCounts) / ((double) sizeOfHistogram) is the average height of the histogram
    // avg * epsilon defines the threshold for a flat histogram
    if (Histogram::minimumValue(lowerBound, upperBound) > (((double) numberOfCounts) / ((double) sizeOfHistogram) * epsilon)) {
        // the histogram is flat enough
        return (true);
    } else {
        // the histogram is not flat enough
        return (false);
    }
}

/**
 * Test whether all bins of the histogram are non zero.
 *
 * @param lowerBound, the minimum value for accepting the step.
 * @param upperBound, the maximum value for accepting the step.
 *
 * @return True: All bins are non zero; false: There exist bin(s) that are zero.
 */
bool Histogram::areAllBinsNonZero(int lowerBound, int upperBound) {
    for (int i = (lowerBound - 1); i < upperBound; i++) {
        if (histogram[i] == 0) {
            return(false);
        }
    }
    return(true);
}

/**
 * Search for the height of the smallest bin of the histogram.
 *
 * @param lowerBound, the minimum value for accepting the step.
 * @param upperBound, the maximum value for accepting the step.
 *
 * @return the height of the smallest bin.
 */
int Histogram::minimumValue(int lowerBound, int upperBound) {
    int smallestBin = histogram[(lowerBound - 1)];
    for (int i = (lowerBound - 1); i < upperBound; i++) {
        if (histogram[i] < smallestBin) {
            smallestBin = histogram[i];
        }
    }
    return(smallestBin);
}
