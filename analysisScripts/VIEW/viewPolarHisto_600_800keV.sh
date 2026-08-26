#!/bin/bash

cd ..
eog $(find . -mindepth 3 -maxdepth 3 -name 'polarHisto_600_800keV*.png')
