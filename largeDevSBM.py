# largeDevSBM.py
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

# Usage:
# python largeDevSBM.py "binary of the program to execute or R for analysis" "disp = interactive mode, nodisp = no interactive mode" "parallel = run binary parallel, sequentiell = run program sequentiell, analyse = prepare an R script" < params

from subprocess import Popen, PIPE
from multiprocessing import Pool
from prepareSimulations import prepareSimulation
from prepareAnalysis import prepareAnalysis
from sys import argv, exit

## call the program
def runProgram(arguments):
	Process = Popen(argv[1], shell = True, stdin = PIPE, stderr = PIPE)
	result = Process.communicate(arguments)
	if (argv[2] == "disp"):
		print(result)

if __name__ == "__main__":
	if (len(argv) < 4):
		exit("Not enough arguments")
	
	disp = False
	if (argv[2] == "disp"):
		disp = True
	
	# provide the parameters
	args = prepareSimulation(disp)
	
	# run the program for the given configurations
	if (argv[3] == "parallel"):
		p = Pool()
		p.map(runProgram, args)
	elif (argv[3] == "sequentiell"):
		for arg in args:
			runProgram(arg)
	elif (argv[3] == "analyse"):
		prepareAnalysis(args)
	else:
		exit("Invalid parameter for the type of work, allowed: parallel, sequentiell or analyse")
	
	print('\n\n')
