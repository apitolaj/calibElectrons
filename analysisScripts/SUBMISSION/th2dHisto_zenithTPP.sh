#!/bin/bash
#SBATCH --job-name=th2dHisto_allEnergies
#SBATCH --mem=1G
#SBATCH --licenses=sps
#SBATCH --time=00:10:00
#SBATCH --output=/dev/null
#SBATCH --error=/dev/null
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --export=HOME,USER,THRONG_DIR

source "${PWD}/../../directory_template/config.sh"

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
    
        DST_DIR="../../SOURCES/Source_${i}_${k}/ANALYSIS"
        
        if [ -d ${DST_DIR} ]; then
        
		sed "s|SOURCE_PLACEHOLDER|${i}_${k}|g" "../PLOT/th2dHisto_zenithTPP.cpp" > "${DST_DIR}/th2dHisto_zenithTPP_Source_${i}_${k}.cpp"
		
		(cd ${DST_DIR} && root -q -l -b 'th2dHisto_zenithTPP_Source_'${i}_${k}'.cpp("analysisROOT_Source_'${i}_${k}'.root")')
		
	fi
	
    done
done
