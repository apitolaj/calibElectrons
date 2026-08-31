#!/bin/bash
#SBATCH --job-name=curveFitting
#SBATCH --output=/dev/null
#SBATCH --error=/dev/null
#SBATCH --time=05:00:00
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

sed "s|SOURCE_PLACEHOLDER|Source_${i}_${k}|g" "../PLOT/curveFittingGauss.cpp" > "${DST_DIR}/curveFittingGauss_Source_${i}_${k}.cpp"

sed "s|SOURCE_PLACEHOLDER|Source_${i}_${k}|g" "../PLOT/curveFittingLangau.cpp" > "${DST_DIR}/curveFittingLangau_Source_${i}_${k}.cpp"

(cd "${DST_DIR}" && root -l -b -q 'curveFittingGauss_Source_'${i}_${k}'.cpp("analysisROOT_Source_'${i}_${k}'.root")')
(cd "${DST_DIR}" && root -l -b -q 'curveFittingLangau_Source_'${i}_${k}'.cpp("analysisROOT_Source_'${i}_${k}'.root")')

echo "Finished source_${i}_${k}."
