#!/bin/bash

cd ..
eog $(find . -mindepth 3 -maxdepth 3 -name 'polarHisto_allEnergies*.png')
