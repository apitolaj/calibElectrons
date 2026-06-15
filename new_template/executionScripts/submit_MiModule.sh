#!/bin/bash
set -euo pipefail

SRC_BASE="/sps/nemo/scratch/apitolaj/new_template/DATA/brios/SNCUTS/"
DST_BASE="/sps/nemo/scratch/apitolaj/new_template/DATA/brios/SNCUTS/ROOT/"
mkdir -p "${DST_BASE}"

LIST="${DST_BASE}/inputted_brio_list.txt"
find "${SRC_BASE}" -mindepth 2 -maxdepth 2 -type f \
  -name "2D_reco_Bi_*.brio" \
  | sort > "${LIST}"

cp /sps/nemo/scratch/apitolaj/new_template/DATA/brios/SNCUTS/ROOT/inputted_brio_list.txt /sps/nemo/scratch/apitolaj/new_template/DATA/brios/SNCUTS/ROOT/root_list.txt

FILE="/${DST_BASE}/root_list.txt"
sed 's|/SNCUTS/|/SNCUTS/ROOT/|g; s|\.brio$|\.root|g' "${LIST}" > "${FILE}"

N=$(wc -l < "${LIST}")
echo "Found ${N} brio files."
[ "${N}" -gt 0 ] || { echo "No brio files found."; exit 1; }

WORKER="/sps/nemo/scratch/apitolaj/new_template/executionScripts/run_MiModule.sh"
chmod +x "${WORKER}"
[ -f "${WORKER}" ] || { echo "ERROR: worker not found: ${WORKER}"; exit 4; }
[ -x "${WORKER}" ] || { echo "ERROR: worker not executable: ${WORKER}"; exit 5; }

# Submit the array job and capture its job ID
ARRAY_JOB_ID=$(sbatch --array=1-"${N}"%400 \
       --output="${DST_BASE}/%a/slurm-miroot-%A_%a.out" \
       --error="/dev/null" \
       --parsable \
       "${WORKER}" "${LIST}" "${SRC_BASE}" "${DST_BASE}")

echo "Array job submitted: ${ARRAY_JOB_ID}"

STACK_NAME="falaise@2026-04-07"

# Submit the follow-up job, only runs after ALL array tasks complete successfully
FOLLOWUP_JOB_ID=$(sbatch --dependency=afterok:${ARRAY_JOB_ID} \
       --job-name=mimerge \
       --mem=2G \
       --licenses=sps \
       --time=00:10:00 \
       --ntasks=1 \
       --cpus-per-task=1 \
       --output="/dev/null" \
       --error="/dev/null" \
       --parsable \
       --wrap="source ${THRONG_DIR:-}/config/supernemo_profile.bash && snswmgr_load_stack ${STACK_NAME} && root -l -b -q 'merge.cpp(\"/sps/nemo/scratch/apitolaj/new_template/DATA/ROOTFiles/merged_output_${ARRAY_JOB_ID}.root\",\"${DST_BASE}/root_list.txt\")'")

echo "Follow-up job submitted: ${FOLLOWUP_JOB_ID} (runs after ${ARRAY_JOB_ID})"
