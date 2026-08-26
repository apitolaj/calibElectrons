#!/bin/bash
# submit_simu.sh
#
# Submits a SLURM array job for Bi-207 flsimulate + flreconstruct.
# Same pattern as submit_calibrate_using_BOTH-const_array.sh:
#   - this script decides HOW MANY jobs to run
#   - passes SIMU_DATA to the worker as argument
#   - SLURM_ARRAY_TASK_ID is used as num (folder index) inside worker
#
# Usage:
#   bash submit_simu.sh FIRST LAST
#
# Example ? simulate events 1..100, each in its own folder:
#   bash submit_simu.sh 1 100

source "${PWD}/config.sh"

set -euo pipefail

sed "s|HOMEDIR_PLACEHOLDER|${HOME_PATH}|g; s|SOURCE_PLACEHOLDER|${SOURCE}|g" "${HOME_PATH}/configuration/simu_setup.conf.template" > "${MY_DIR}/simu_setups/simu_setup_source_${SOURCE}.conf"

sed "s|HOMEDIR_PLACEHOLDER|${HOME_PATH}|g; s|SOURCE_PLACEHOLDER|${SOURCE}|g" "${HOME_PATH}/configuration/1S-pipeline.conf.template" > "${MY_DIR}/1S-pipelines/1S-pipeline_source_${SOURCE}.conf"

# ============================================================
# ARGUMENTS
# ============================================================
[ $# -eq 2 ] || { echo "ERROR: Usage: $0 FIRST LAST"; exit 2; }

FIRST="$1"
LAST="$2"

[ "${FIRST}" -ge 1 ]         || { echo "ERROR: FIRST must be >= 1"; exit 3; }
[ "${LAST}" -ge "${FIRST}" ] || { echo "ERROR: LAST must be >= FIRST"; exit 3; }

# ============================================================
# PATHS
# ============================================================

# included in config.sh
# SIMU_DATA="/sps/nemo/scratch/apitolaj/new_template/DATA/brios"
mkdir -p "${SIMU_DATA}"

WORKER="$(cd "$(dirname "$0")" && pwd)/run_simu.sh"

[ -f "${WORKER}" ] || { echo "ERROR: worker script not found: ${WORKER}"; exit 4; }
[ -x "${WORKER}" ] || { echo "ERROR: worker script is not executable: ${WORKER}"; exit 5; }

N=$(( LAST - FIRST + 1 ))

echo "========================================"
echo "Submitting Bi-207 simulation array"
echo "FIRST    : ${FIRST}"
echo "LAST     : ${LAST}"
echo "N jobs   : ${N}"
echo "SIMU_DATA: ${SIMU_DATA}"
echo "WORKER   : ${WORKER}"
echo "========================================"

# ============================================================
# SUBMIT ? same pattern as submit_calibrate:
#   sbatch --array=... WORKER ARG1 ARG2 ...
# Max 50 simultaneous jobs ? adjust %50 as needed
# ============================================================
for i in $(seq "${FIRST}" "${LAST}"); do
    mkdir -p "${SIMU_DATA}/${i}"
done

# included in config.sh
# SNCUTS_SCRIPT="/sps/nemo/scratch/apitolaj/new_template/executionScripts/submit_SNCUTS_on_c2e_array.sh"

ARRAY_JOB_ID=$(sbatch --parsable \
       --array="${FIRST}-${LAST}%400" \
       --output="${SIMU_DATA}/%a/slurm-simu-%A_%a.out" \
       --error="${SIMU_DATA}/%a/slurm-simu-%A_%a.err" \
       "${WORKER}" "${SIMU_DATA}")

echo "Array job submitted: ${ARRAY_JOB_ID}"
echo "Submitted array job: indices ${FIRST} to ${LAST} (max 50 simultaneous)."
echo "Output brios: ${SIMU_DATA}/<num>/2D_reco_Bi_<num>.brio"

FOLLOWUP_JOB_ID=$(sbatch --parsable \
       --dependency=afterok:${ARRAY_JOB_ID} \
       --mem=1G \
       --licenses=sps \
       --time=00:02:00 \
       --output="/dev/null" \
       --error="/dev/null" \
       --ntasks=1 \
       --cpus-per-task=1 \
       "${SNCUTS_SCRIPT}")

echo "Follow-up job submitted: ${FOLLOWUP_JOB_ID} (runs after ${ARRAY_JOB_ID})"


