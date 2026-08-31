#!/bin/bash
# Scans SOURCES/ for Source_<i>_<k> dirs and submits the array job.
set -euo pipefail

BASE_DIR="$(pwd)/../.."
SOURCES_DIR="${BASE_DIR}/SOURCES"

TASKLIST="${SOURCES_DIR}/tasklist.txt"
> "${TASKLIST}"

for dir in "${SOURCES_DIR}"/Source_*_*; do
    [ -d "${dir}/DATA/ROOTFiles" ] || continue
    name=$(basename "${dir}")     # Source_i_k
    ik="${name#Source_}"          # i_k
    i="${ik%%_*}"
    k="${ik#*_}"
    echo "${i} ${k}" >> "${TASKLIST}"
done

N=$(wc -l < "${TASKLIST}")
if [ "${N}" -eq 0 ]; then
    echo "No matching source directories found in ${SOURCES_DIR}" >&2
    exit 1
fi

echo "Found ${N} source directories -> submitting array 1-${N}"
sbatch --array=1-"${N}" --export=ALL,THRONG_DIR,TASKLIST="${TASKLIST}" run_curveFitting.sh
