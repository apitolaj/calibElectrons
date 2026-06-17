#!/bin/bash

for i in $(seq 0 5);
do
    for k in $(seq 0 6);
    do
    
        DST_DIR="angles_Source_${i}_${k}"
        
        sed "s|SOURCE_PLACEHOLDER|${i}_${k}|g" "plotAngles.cpp" > "${DST_DIR}/plotAngles_Source_${i}_${k}.cpp"
        
	(cd angles_Source_${i}_${k} && root -q -l -b 'plotAngles_Source_'${i}_${k}'.cpp("angles_envelope_Source_'${i}_${k}'.root","angles_noEnvelope_Source_'${i}_${k}'.root")')
	
    done
done
