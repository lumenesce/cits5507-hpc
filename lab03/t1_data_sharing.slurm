#!/bin/bash

#SBATCH --job-name=t1_data_sharing
#SBATCH --output=t1_data_sharing.out
#SBATCH --error=t1_data_sharing.err

#SBATCH --cpus-per-task=4
#SBATCH --time=00:00:30
#SBATCH --mem=1G
#SBATCH --partition=cits3402

echo "Executed using optimiser flag -O3 -----------------\n"
gcc t1_data_sharing.c -fopenmp -fopt-info-vec -O3 -o t1_data_sharing && ./t1_data_sharing