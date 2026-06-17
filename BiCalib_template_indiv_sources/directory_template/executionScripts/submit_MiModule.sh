#!/bin/bash

source config.sh
source home_path.sh

set -euo pipefail

# included in config.sh
# SRC_BASE_MI="/sps/nemo/scratch/apitolaj/new_template/DATA/brios/SNCUTS/"
# DST_BASE_MI="/sps/nemo/scratch/apitolaj/new_template/DATA/brios/SNCUTS/ROOT/"

mkdir -p "${DST_BASE_MI}"

LIST="${DST_BASE_MI}/inputted_brio_list.txt"
find "${SRC_BASE_MI}" -mindepth 2 -maxdepth 2 -type f \
  -name "2D_reco_Bi_*.brio" \
  | sort > "${LIST}"

#maybe makek the path go through DST_BASE_MI
cp ${DST_BASE_MI}/inputted_brio_list.txt ${DST_BASE_MI}/root_list.txt

FILE="/${DST_BASE_MI}/root_list.txt"
sed 's|/SNCUTS/|/SNCUTS/ROOT/|g; s|\.brio$|\.root|g' "${LIST}" > "${FILE}"

N=$(wc -l < "${LIST}")
echo "Found ${N} brio files."
[ "${N}" -gt 0 ] || { echo "No brio files found."; exit 1; }

#included in config.sh
#WORKER_MI="/sps/nemo/scratch/apitolaj/new_template/executionScripts/run_MiModule.sh"
chmod +x "${WORKER_MI}"
[ -f "${WORKER_MI}" ] || { echo "ERROR: worker not found: ${WORKER_MI}"; exit 4; }
[ -x "${WORKER_MI}" ] || { echo "ERROR: worker not executable: ${WORKER_MI}"; exit 5; }

# Submit the array job and capture its job ID
ARRAY_JOB_ID=$(sbatch --array=1-"${N}"%400 \
       --output="${DST_BASE_MI}/%a/slurm-miroot-%A_%a.out" \
       --error="/dev/null" \
       --parsable \
       "${WORKER_MI}" "${LIST}" "${SRC_BASE_MI}" "${DST_BASE_MI}")

echo "Array job submitted: ${ARRAY_JOB_ID}"

STACK_NAME="falaise@2026-04-07"

mkdir -p "${HOME_PATH}/${SOURCE_FAMILY}/DATA/ROOTFiles/logs"

# Submit the follow-up job, only runs after ALL array tasks complete successfully
FOLLOWUP_JOB_ID=$(sbatch --dependency=afterok:${ARRAY_JOB_ID} \
       --job-name=mimerge \
       --mem=2G \
       --licenses=sps \
       --time=00:10:00 \
       --ntasks=1 \
       --cpus-per-task=1 \
       --output="${HOME_PATH}/${SOURCE_FAMILY}/DATA/ROOTFiles/logs/merged_${SOURCE_FAMILY}_${ARRAY_JOB_ID}.out" \
       --error="/dev/null" \
       --parsable \
       --wrap="source ${THRONG_DIR:-}/config/supernemo_profile.bash && snswmgr_load_stack ${STACK_NAME} && root -l -b -q 'merge.cpp(\"${HOME_PATH}/${SOURCE_FAMILY}/DATA/ROOTFiles/merged_${SOURCE_FAMILY}_${ARRAY_JOB_ID}.root\",\"${DST_BASE_MI}/root_list.txt\")'")

echo "Follow-up job submitted: ${FOLLOWUP_JOB_ID} (runs after ${ARRAY_JOB_ID})"
