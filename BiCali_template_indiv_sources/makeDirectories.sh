#!/bin/bash

for i in $(seq 0 1);
do
    for k in $(seq 0 1);
    do
        cp -r directory_template Source_${i}_${k}
        cd Source_${i}_${k}/executionScripts
        sed -i "s|SOURCE_PLACEHOLDER|${i}_${k}|g" "config.sh"
        cd ../..
    done
done
