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

FIRST=1
LAST=1

for i in $(seq 0 5);
do
    for k in $(seq 0 6);
    do
	(cd "${HOME_PATH}/SOURCES/Source_${i}_${k}" && bash submit_simu_array.sh ${FIRST} ${LAST})
    done
done
