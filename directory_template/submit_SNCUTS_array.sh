#!/bin/bash

source "${PWD}/config.sh"

set -euo pipefail

# included in config.sh
# SRC_BASE="/sps/nemo/scratch/apitolaj/new_template/DATA/brios"
# DST_BASE="/sps/nemo/scratch/apitolaj/new_template/DATA/brios/SNCUTS/"

#sed "s|HOMEDIR_PLACEHOLDER|${HOME_PATH}|g" "${HOME_PATH}/configuration/SNCUTS-pipeline.conf.template" > "${HOME_PATH}/configuration/SNCUTS-pipelines/SNCuts-pipeline_source_${SOURCE}.conf"

mkdir -p "${DST_BASE_SN}"
LIST="${DST_BASE_SN}/inputted_brio_list.txt"

# Numbered folders: brios/1/, brios/2/, ... each contains 2D_reco_Bi_N.brio
find "${SRC_BASE_SN}" -mindepth 2 -maxdepth 2 -type f \
  -name "2D_reco_Bi_*.brio" \
  | sort > "${LIST}"

N=$(wc -l < "${LIST}")
echo "Found ${N} brio files."
[ "${N}" -gt 0 ] || { echo "No 2D_reco_Bi brio files found."; exit 1; }

# included in config.sh
# WORKER="/sps/nemo/scratch/apitolaj/new_template/executionScripts/run_SNCUTS_on_c2e_array.sh"
chmod +x "${WORKER_SN}"

[ -f "${WORKER_SN}" ] || { echo "ERROR: worker script not found: ${WORKER_SN}"; exit 4; }
[ -x "${WORKER_SN}" ] || { echo "ERROR: worker script is not executable: ${WORKER_SN}"; exit 5; }

for i in $(seq 1 "${N}"); do
    RUNID=$(sed -n "${i}p" "${LIST}" | xargs dirname | xargs basename)
    mkdir -p "${DST_BASE_SN}/${RUNID}"
done

#included in config.sh
#MIMODULE_SCRIPT="/sps/nemo/scratch/apitolaj/new_template/executionScripts/submit_MiModule.sh"

ARRAY_JOB_ID=$(sbatch --parsable \
       --array=1-"${N}"%400 \
       --output="/dev/null" \
       --error="/dev/null" \
       "${WORKER_SN}" "${LIST}" "${SRC_BASE_SN}" "${DST_BASE_SN}")

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
       

