#!/bin/bash
#SBATCH --job-name=th2dHisto_600_800keV
#SBATCH --mem=1G
#SBATCH --licenses=sps
#SBATCH --time=00:10:00
#SBATCH --output=/dev/null
#SBATCH --error=/dev/null
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --export=HOME,USER,THRONG_DIR

PROFILE_SCRIPT="${THRONG_DIR}/config/supernemo_profile.bash"
STACK_NAME="falaise@2026-04-07"

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
        
        if [ -d ${DST_DIR} ]; then
        
		sed "s|SOURCE_PLACEHOLDER|${i}_${k}|g; s|ENERGY_PLACEHOLDER1|600_800keV|g; s|ENERGY_PLACEHOLDER2|600-800keV|g" "../ROOT/th2dHisto_zenithDistOM.cpp" > "${DST_DIR}/th2dHisto_zenithDistOM_600_800keV_Source_${i}_${k}.cpp"
		
		(cd ${DST_DIR} && root -q -l -b 'th2dHisto_zenithDistOM_600_800keV_Source_'${i}_${k}'.cpp("zenithDistOM_envelope_600_800keV_Source_'${i}_${k}'.root","zenithDistOM_noEnvelope_600_800keV_Source_'${i}_${k}'.root")')
		
	fi
	
    done
done
