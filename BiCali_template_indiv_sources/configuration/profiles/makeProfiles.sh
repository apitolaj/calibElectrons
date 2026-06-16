#!/bin/bash

for i in $(seq 0 5);
do
    for k in $(seq 0 6);
    do
        sed "s|SOURCE_NUM|${i}_${k}|g" "simu_source.profile.template" > "testsim_source_${i}_${k}.profile"
    done
done
