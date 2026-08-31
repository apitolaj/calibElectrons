#!/bin/bash

sbatch polarHisto.sh
sbatch th2dHisto_TPPDistribution.sh
sbatch th2dHisto_zenithDistOM.sh
sbatch th2dHisto_zenithTPP.sh

bash submit_curveFitting.sh
