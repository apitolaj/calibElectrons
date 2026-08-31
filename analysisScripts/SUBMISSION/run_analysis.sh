#!/bin/bash
#SBATCH --job-name=analysis
#SBATCH --output=/dev/null
#SBATCH --error=/dev/null
#SBATCH --time=00:30:00
#SBATCH --mem=2G
#SBATCH --cpus-per-task=1

source "${PWD}/../../directory_template/config.sh"

LINE=$(sed -n "${SLURM_ARRAY_TASK_ID}p" "${TASKLIST}")
i=$(awk '{print $1}' <<< "${LINE}")
k=$(awk '{print $2}' <<< "${LINE}")

BASE_DIR="$(pwd)/../.."
DST_DIR="${BASE_DIR}/SOURCES/Source_${i}_${k}/ANALYSIS"

mkdir -p ${DST_DIR}

set +e
set +u
source "${PROFILE_SCRIPT}"
PROFILE_RC=$?
snswmgr_load_stack "${STACK_NAME}"
STACK_RC=$?
set -e
set -u

sed "s|SOURCE_PLACEHOLDER|${i}_${k}|g; s|DST_PLACEHOLDER|${DST_DIR}|g; s|BASE_PLACEHOLDER|${BASE_DIR}|g;
s|UTILS_PLACEHOLDER|../../ROOT|g" "../ROOT/analysis_chainROOT.cpp" > "${DST_DIR}/analysis_chainROOT_Source_${i}_${k}.cpp"

(cd "${DST_DIR}" && root -l -b -q "analysis_chainROOT_Source_${i}_${k}.cpp()")

echo "Finished source_${i}_${k}."
