#!/bin/bash
#SBATCH --job-name=polarHisto
#SBATCH --mem=1G
#SBATCH --licenses=sps
#SBATCH --output=output.out
#SBATCH --error=err.er
#SBATCH --time=00:10:00
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --export=HOME,USER,THRONG_DIR

PROFILE_SCRIPT="${THRONG_DIR}/config/supernemo_profile.bash"
STACK_NAME="falaise@2026-06-19"

set +e
set +u
source "${PROFILE_SCRIPT}"
PROFILE_RC=$?
snswmgr_load_stack "${STACK_NAME}"
STACK_RC=$?
set -e
set -u

for i in $(seq 0 5);
do
    for k in $(seq 0 6);
    do
    	
        DST_DIR="../ANALYSIS/Source_${i}_${k}"
        
        mkdir -p "${DST_DIR}"
        
        if [ -d ${DST_DIR} ]; then
        	
		sed "s|SOURCE_PLACEHOLDER|${i}_${k}|g; s|ENERGY_PLACEHOLDER|placeholder|g" "../PLOT/polarHistoTEST_angles.cpp" > "${DST_DIR}/polarHistoTEST_Source_${i}_${k}.cpp"
		
#		(cd ${DST_DIR} && root -q -l -b 'polarHisto_Source_'${i}_${k}'.cpp("analysisROOT.root")')
	
	fi
	
    done
done
