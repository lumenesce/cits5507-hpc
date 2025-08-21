#!/bin/bash

#SBATCH --job-name=usleep
#SBATCH --output=usleep.out
#SBATCH --error=usleep.err

#SBATCH --cpus-per-task=4
#SBATCH --time=00:00:30
#SBATCH --mem=1G
#SBATCH --partition=cits3402

echo "Using optimiser flag -O3 -----------------"
gcc usleep.c -fopenmp -fopt-info-vec -O3 -o usleep && ./usleep