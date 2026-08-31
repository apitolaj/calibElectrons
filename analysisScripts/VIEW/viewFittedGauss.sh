#!/bin/bash

cd ../..
eog $(find . -mindepth 4 -maxdepth 4 -name 'fittedGauss_*.png')
