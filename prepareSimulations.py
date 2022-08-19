# prepareSimulations.py
# Copyright (C) 2016, 2017 largeDevSBM
#
# This file is part of largeDevSBM.
#
# largeDevSBM is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# largeDevSBM is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program.  If not, see <http://www.gnu.org/licenses/>.

from numpy import arange

## Return the given message, if dist = True
def dispMessage(disp, message):
	if (disp):
		return(message)

## Calculate the multiplier for the given value a, such that it becomes an integer.
def getMultiplier(a):
	x = 1
	while (a*x%1 != 0):
		x = x * 10
		
	return(x)

## Calculate the maximum multiplier for the three given values.
def getMaxMultiplier(a, b, c):
	return(max(getMultiplier(a), getMultiplier(b), getMultiplier(c)))

## Process simple sampling.
def simpleSampling(disp, result, initialize):
	samples = raw_input(dispMessage(disp, "number of samples\n")) ################################################## ss1
	result.append(initialize + " 0 " + samples)
	
	return(result)

## Process importance sampling with the Metropolis algorithm.
def metropolisAlgorithm(disp, result, initialize):
	tempMin = float(raw_input(dispMessage(disp, "enter minimum temperature\n"))) ################################## is1
	tempMax = float(raw_input(dispMessage(disp, "enter maximum temperature\n"))) ################################## is2
	tempStep = float(raw_input(dispMessage(disp, "enter temperature step\n"))) #################################### is3
	samples = raw_input(dispMessage(disp, "number of samples\n")) ################################################## is4
	equi = raw_input(dispMessage(disp, "number of steps for equilibration\n")) ##################################### is5
	mult = getMaxMultiplier(tempMin, tempMax, tempStep)
	for temp in arange(tempMin * mult, tempMax * mult, tempStep * mult):
		if (temp != 0):
			result.append(initialize + " 1 " + str(temp / mult) + " " + samples + " " + equi)
	
	return(result)

## Process the Wang-Landau algorithm.
def wangLandauAlgorithm(disp, result, initialize):
	lowerBound = raw_input(dispMessage(disp, "enter lower bound\n")) ############################################### wl1
	upperBound = raw_input(dispMessage(disp, "enter upper bound\n")) ############################################### wl2
	sweepsToEvaluate = raw_input(dispMessage(disp, "enter the number of sweeps for the evaluation\n")) ############# wl3
	modiFacFinal = raw_input(dispMessage(disp, "enter the final modification factor\n")) ########################### wl4
	result.append(initialize + " 3 " + lowerBound + " " + upperBound + " " + sweepsToEvaluate + " " + modiFacFinal)
	
	return(result)

## Verification of equilibration.
def equilibrate(disp, result, initialize):
	tempMin = float(raw_input(dispMessage(disp, "enter minimum temperature\n"))) ################################## eq1
	tempMax = float(raw_input(dispMessage(disp, "enter maximum temperature\n"))) ################################## eq2
	tempStep = float(raw_input(dispMessage(disp, "enter temperature step\n"))) #################################### eq3
	samples = raw_input(dispMessage(disp, "number of samples\n")) ################################################## eq4
	mult = getMaxMultiplier(tempMin, tempMax, tempStep)
	for temp in arange(tempMin * mult, tempMax * mult, tempStep * mult):
		if (temp != 0):
			result.append(initialize + " 2 " + str(temp / mult) + " " + samples)
	
	return(result)

# initialze the system
def initSystem(disp):
	isDigraph = raw_input(dispMessage(disp, "is digraph y/n\n")) ################################################### s1
	loopsAllowed = raw_input(dispMessage(disp, "are loops allowed y/n\n")) ######################################### s2
	directory = raw_input(dispMessage(disp, "enter directory\n")) ################################################## s3
	vertices = raw_input(dispMessage(disp, "enter the number of vertices of the graph\n")) ######################### s4
	generator = raw_input(dispMessage(disp, "enter the number of the generator to use\n")) ######################### s5
	seed = raw_input(dispMessage(disp, "enter a seed for the random number generator\n")) ########################## s6
	value = raw_input(dispMessage(disp, "enter the value to use\n")) ############################################### s7
	graph = raw_input(dispMessage(disp, "enter the number of graph to use\n")) ##################################### s8
	firstConnectivity = secondConnectivity = numberOfBlocks = initialize = ""
	if (int(graph) == 0):
		firstConnectivity = raw_input(dispMessage(disp, "enter connectivity of the graph\n")) ###################### s9
		initialize = isDigraph + " " + loopsAllowed + " " + directory + " " + vertices + " " + generator + " " + seed + " " + value + " " + graph + " " + firstConnectivity
	elif (int(graph) == 1):
		firstConnectivity = raw_input(dispMessage(disp, "enter interblock connectivity of the graph\n")) ########### s10
		secondConnectivity = raw_input(dispMessage(disp, "enter intrablock connectivity of the graph\n")) ########## s11
		numberOfBlocks = raw_input(dispMessage(disp, "enter the number of blocks\n")) ############################## s12
		initialize = isDigraph + " " + loopsAllowed + " " + directory + " " + vertices + " " + generator + " " + seed + " " + value + " " + graph + " " + firstConnectivity + " " + secondConnectivity + " " + numberOfBlocks
	
	return(initialize)

## Read in the the parameters for the simulation.
def doAction(disp, initialize):
	action = 0
	result = []
	while (action != -1):
		action = int(raw_input(dispMessage(disp, "which action should be done\n"))) ################################ a1
		if (action == 0):
			result = simpleSampling(disp, result, initialize)
		if (action == 1):
			result = metropolisAlgorithm(disp, result, initialize)
		if (action == 2):
			result = equilibrate(disp, result, initialize)
		if (action == 3):
			result = wangLandauAlgorithm(disp, result, initialize)
	
	return(result)

## Prepare everything for running a simulation.
def prepareSimulation(disp):
	initialize = initSystem(disp)
	result = doAction(disp, initialize)
	
	return(result)
