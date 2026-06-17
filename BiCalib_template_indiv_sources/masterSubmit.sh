#!/bin/bash

: '
This is the master submission file for generating data for each of the calibration sources individually; this script should only be ran after the makeProfiles.sh and makeDirectories.sh scripts have been ran and the home 
directory of the user has been correctly inputted into home_path.sh. The user should comment out the lines
corresponding to sources that they do not want to simulate.

The two numbers (e.g., 1 1, for the original version of the script) correspond to the first and last job
numbers that the user wishes to run for each source; in each source folder, numbered directories will appear
that will hold the log information for the simulation and a copy of the pipeline configuration files pased into Falaise for documentation. 

Assuming no errors, the final output of each of the submissions below will be a ROOT file in the ROOTFiles
folder in each of the source folders. These ROOT files can be utilized for analysis as documented in the rest
of the calibElectrons repository found at https://github.com/apitolaj/calibElectrons.git.

In order to not overwrite numbered directories, users should be careful to make sure that each new submission of this script has the first number submitted in each command as one-greater than the original second number.

Note: DO NOT exceed 200 million events being simulated concurrently at any point across all sources as the temporary brio files that are created may overflood the disk space on the superNEMO shared directory.

Once all is set, simply run the script: bash masterSubmit.sh

'

source home_path.sh

	(cd "${HOME_PATH}/Source_0_0/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_0_1/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_0_2/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_0_3/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_0_4/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_0_5/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_0_6/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_1_0/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_1_1/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_1_2/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_1_3/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_1_4/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_1_5/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_1_6/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_2_0/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_2_1/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_2_2/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_2_3/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_2_4/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_2_5/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_2_6/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_3_0/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_3_1/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_3_2/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_3_3/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_3_4/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_3_5/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_3_6/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_4_0/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_4_1/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_4_2/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_4_3/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_4_4/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_4_5/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_4_6/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_5_0/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_5_1/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_5_2/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_5_3/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_5_4/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_5_5/executionScripts" && bash submit_simu_array.sh 1 1)
	(cd "${HOME_PATH}/Source_5_6/executionScripts" && bash submit_simu_array.sh 1 1)







































