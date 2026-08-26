#!/bin/bash

DST_DIR=SOURCES

mkdir -p "${DST_DIR}"

initialize()
{
    local row="$1"
    local col="$2"

    cp -r directory_template "${DST_DIR}/Source_${row}_${col}"
    cd "${DST_DIR}/Source_${row}_${col}" 
    sed -i "s|SOURCE_PLACEHOLDER|${row}_${col}|g" "config.sh"
    cd ../..
}

for i in $(seq 0 5); do
    for k in $(seq 0 6); do
        initialize "${i}" "${k}"
    done
done

cd configuration/profiles
bash makeProfiles.sh
cd ../..