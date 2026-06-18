#!/bin/bash

BASE_DIR="$(pwd)"

for i in $(seq 0 5);
do
    for k in $(seq 0 6);
    do
    	DST_DIR="angles_Source_${i}_${k}"
    	mkdir -p ${DST_DIR}
    	
	sed "s|SOURCE_PLACEHOLDER|${i}_${k}|g; s|DST_PLACEHOLDER|${DST_DIR}|g; s|BASE_PLACEHOLDER|${BASE_DIR}|g" "calculateAngles_chainROOT.cpp" > "${DST_DIR}/calculateAngles_chainROOT_Source_${i}_${k}.cpp"
	
	(cd "../Source_${i}_${k}/DATA/ROOTFiles" && root -l -b -q "${BASE_DIR}/${DST_DIR}/calculateAngles_chainROOT_Source_${i}_${k}.cpp")
	
	echo "Finished source_${i}_${k}."

    done
done
