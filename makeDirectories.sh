#!/bin/bash

DST_DIR=SOURCES

mkdir -p ${DST_DIR}

for i in $(seq 0 5);
do
    for k in $(seq 0 6);
    do
        cp -r directory_template "${DST_DIR}/Source_${i}_${k}"
        cd ${DST_DIR}/Source_${i}_${k}
        sed -i "s|SOURCE_PLACEHOLDER|${i}_${k}|g" "config.sh"
        cd ../..
    done
done
