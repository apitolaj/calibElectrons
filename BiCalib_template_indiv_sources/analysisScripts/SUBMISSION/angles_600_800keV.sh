#!/bin/bash
#SBATCH --job-name=ang_600_800keV
#SBATCH --array=1-42
#SBATCH --output=output.out
#SBATCH --error=/dev/null
#SBATCH --time=00:30:00
#SBATCH --mem=1G
#SBATCH --cpus-per-task=1
#SBATCH --export=HOME,USER,THRONG_DIR

IDX=$((SLURM_ARRAY_TASK_ID - 1))
i=$((IDX / 7))
k=$((IDX % 7))

BASE_DIR="$(pwd)/.."
DST_DIR="ANALYSIS/Source_${i}_${k}"

mkdir -p ${BASE_DIR}/${DST_DIR}

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

sed "s|SOURCE_PLACEHOLDER|${i}_${k}|g; s|DST_PLACEHOLDER|${DST_DIR}|g; s|BASE_PLACEHOLDER|${BASE_DIR}|g; 
s|UTILS_PLACEHOLDER|../../ROOT|g" "../ROOT/angles_600_800keV_chainROOT.cpp" > "../${DST_DIR}/angles_600_800keV_chainROOT_Source_${i}_${k}.cpp"
	
(cd "../../SOURCES/Source_${i}_${k}/DATA/ROOTFiles" && root -l -b -q "${BASE_DIR}/${DST_DIR}/angles_600_800keV_chainROOT_Source_${i}_${k}.cpp")
	
echo "Finished source_${i}_${k}."

