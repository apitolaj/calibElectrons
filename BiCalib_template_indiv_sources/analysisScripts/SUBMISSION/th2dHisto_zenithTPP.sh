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
        
		sed "s|SOURCE_PLACEHOLDER|${i}_${k}|g; s|ENERGY_PLACEHOLDER1|allEnergies|g; s|ENERGY_PLACEHOLDER2|all energies|g" "../ROOT/th2dHisto_zenithTPP.cpp" > "${DST_DIR}/th2dHisto_zenithTPP_allEnergies_Source_${i}_${k}.cpp"
		
		(cd ${DST_DIR} && root -q -l -b 'th2dHisto_zenithTPP_allEnergies_Source_'${i}_${k}'.cpp("zenithTPP_envelope_allEnergies_Source_'${i}_${k}'.root","zenithTPP_noEnvelope_allEnergies_Source_'${i}_${k}'.root")')
		
	fi
	
    done
done
