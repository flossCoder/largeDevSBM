# prepareAnalysis.py
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

from os.path import exists
from os import rename
from sys import exit

## Back up the old version of the settings script.
def backUp(directory):
	if (exists(directory + "/settings.R")):
		counter = 0
		while (exists(directory + "/settings_bak_" + str(counter) + ".R")):
			counter = counter + 1
		rename(directory + "/settings.R", directory + "/settings_bak_" + str(counter) + ".R")

## Save the settings script for the current parameters.
def saveSetting(directory, numberOfVertices, numberOfGraphs, dataMetropolis, dataWangLandau):
	file = open(directory + "/settings.R", "w")
	
	file.write('directory = "' + directory + '"\n\n')
	file.write('numberOfVertices = ' + numberOfVertices + '\n')
	file.write('numberOfGraphs = ' + numberOfGraphs + '\n\n')
	
	# save the stuff for the Metropolis algorithm
	if (dataMetropolis != []):
		file.write('dataMetropolis <- matrix(NA, nrow=' + str(len(dataMetropolis)) + ', ncol=2)\n')
		for i in range(len(dataMetropolis)):
			file.write('dataMetropolis[' + str(i + 1) + ', 1] <- ' + str(dataMetropolis[i][0]) + '\n')
			file.write('dataMetropolis[' + str(i + 1) + ', 2] <- ' + str(dataMetropolis[i][1]) + '\n')
	
	file.write('\n')
	
	# save the stuff for the Wang-Landau algorithm
	if (dataWangLandau != []):
		file.write('dataWangLandau <- matrix(NA, nrow=' + str(len(dataWangLandau)) + ', ncol=2)\n')
		for i in range(len(dataWangLandau)):
			file.write('dataWangLandau[' + str(i + 1) + ', 1] <- ' + str(dataWangLandau[i][0]) + '\n')
			file.write('dataWangLandau[' + str(i + 1) + ', 2] <- ' + str(dataWangLandau[i][1]) + '\n')
	
	file.close()

## Prepare the analysis starting script.
def prepareAnalysis(arguments):
	# prepare data
	directory = None
	numberOfVertices = None
	numberOfGraphs = None
	indexDelta = None
	dataMetropolis = []
	dataWangLandau = []
	indexDelta = None
	for arg in arguments:
		arg = arg.split()
		if (arg[7] == "0"):
			# ER graph
			indexDelta = 9
		elif (arg[7] == "1"):
			# SBM graph
			indexDelta = 11
		if (arg[indexDelta] == "0"):
			# treat simple sampling
			directory = arg[2]
			numberOfVertices = arg[3]
			numberOfGraphs = arg[indexDelta + 1]
		elif (arg[indexDelta] == "1"):
			# treat Metropolis
			dataMetropolis.append([arg[indexDelta + 1], arg[indexDelta + 2]])
		elif (arg[indexDelta] == "3"):
			# treat Wang-Landau
			dataWangLandau.append([arg[indexDelta + 1], arg[indexDelta + 2]])
	
	# test whether we should proceed
	if (directory == None):
		# invalid directory => probably invalid simple sampling => stop
		exit("Invalid argument for directory")		
	
	# back up old settings script (if necessary)
	backUp(directory)
	# save the current settings
	saveSetting(directory, numberOfVertices, numberOfGraphs, dataMetropolis, dataWangLandau)
