#!/bin/bash

cd ../..
eog $(find . -mindepth 4 -maxdepth 4 -name 'polarHisto_angles_ratios_*.png')
