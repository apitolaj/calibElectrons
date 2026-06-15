#!/bin/bash
set -euo pipefail

SRC_BASE="/sps/nemo/scratch/apitolaj/new_template/DATA/brios"
DST_BASE="/sps/nemo/scratch/apitolaj/new_template/DATA/brios/SNCUTS/"

mkdir -p "${DST_BASE}"
LIST="${DST_BASE}/inputted_brio_list.txt"

# Numbered folders: brios/1/, brios/2/, ... each contains 2D_reco_Bi_N.brio
find "${SRC_BASE}" -mindepth 2 -maxdepth 2 -type f \
  -name "2D_reco_Bi_*.brio" \
  | sort > "${LIST}"

N=$(wc -l < "${LIST}")
echo "Found ${N} brio files."
[ "${N}" -gt 0 ] || { echo "No 2D_reco_Bi brio files found."; exit 1; }

WORKER="/sps/nemo/scratch/apitolaj/new_template/executionScripts/run_SNCUTS_on_c2e_array.sh"
chmod +x "${WORKER}"

[ -f "${WORKER}" ] || { echo "ERROR: worker script not found: ${WORKER}"; exit 4; }
[ -x "${WORKER}" ] || { echo "ERROR: worker script is not executable: ${WORKER}"; exit 5; }

for i in $(seq 1 "${N}"); do
    RUNID=$(sed -n "${i}p" "${LIST}" | xargs dirname | xargs basename)
    mkdir -p "${DST_BASE}/${RUNID}"
done

MIMODULE_SCRIPT="/sps/nemo/scratch/apitolaj/new_template/executionScripts/submit_MiModule.sh"

ARRAY_JOB_ID=$(sbatch --parsable \
       --array=1-"${N}"%400 \
       --output="${DST_BASE}/%a/slurm-sncuts-%A_%a.out" \
       --error="/dev/null" \
       "${WORKER}" "${LIST}" "${SRC_BASE}" "${DST_BASE}")

echo "Array job submitted: ${ARRAY_JOB_ID}"

FOLLOWUP_JOB_ID=$(sbatch --parsable \
       --dependency=afterok:${ARRAY_JOB_ID} \
       --mem=1G \
       --licenses=sps \
       --time=00:03:00 \
       --output="/dev/null" \
       --error="/dev/null" \
       --ntasks=1 \
       --cpus-per-task=1 \
       "${MIMODULE_SCRIPT}")

echo "Follow-up job submitted: ${FOLLOWUP_JOB_ID} (runs after ${ARRAY_JOB_ID})"
       

