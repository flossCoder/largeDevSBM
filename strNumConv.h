/*
 * strNumConv.h
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

#ifndef STRNUMCONV_H_INCLUDED
#define STRNUMCONV_H_INCLUDED

#include <sstream>

/**
 * This template function converts a given number to string
 *
 * @param number The given number.
 *
 * @return The converted string.
 *
 * @exception Return empty string.
 */
template <typename T>
    std::string numToStr (T number) {
        try{
            std::ostringstream stringStream;
            stringStream << number;
            return stringStream.str();
        } catch (int e) {
            return "";
        }
    }

#endif // STRNUMCONV_H_INCLUDED
