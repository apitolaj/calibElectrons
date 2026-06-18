#!/bin/bash
#SBATCH --job-name=ANGLES
#SBATCH --array=1-42
#SBATCH --output=output.out
#SBATCH --error=/dev/null
#SBATCH --time=00:03:00
#SBATCH --mem=1G
#SBATCH --cpus-per-task=1

IDX=$((SLURM_ARRAY_TASK_ID - 1))
i=$((IDX / 7))
k=$((IDX % 7))

BASE_DIR="$(pwd)"
DST_DIR="angles_Source_${i}_${k}"

mkdir -p ${DST_DIR}
    	
sed "s|SOURCE_PLACEHOLDER|${i}_${k}|g; s|DST_PLACEHOLDER|${DST_DIR}|g; s|BASE_PLACEHOLDER|${BASE_DIR}|g" "calculateAngles_600_800keV_chainROOT.cpp" > "${DST_DIR}/calculateAngles_600_800keV_chainROOT_Source_${i}_${k}.cpp"
	
(cd "../Source_${i}_${k}/DATA/ROOTFiles" && root -l -b -q "${BASE_DIR}/${DST_DIR}/calculateAngles_600_800keV_chainROOT_Source_${i}_${k}.cpp")
	
echo "Finished source_${i}_${k}."

