#!/bin/bash

#SBATCH --job-name=loop_worksharing
#SBATCH --output=loop_worksharing.out
#SBATCH --error=loop_worksharing.err

#SBATCH --cpus-per-task=8
#SBATCH --time=00:00:30
#SBATCH --mem=1G
#SBATCH --partition=cits3402

echo "Using optimiser flag -O3 -----------------"
gcc loop_worksharing.c -fopenmp -fopt-info-vec -O3 -o loop_worksharing && ./loop_worksharing

echo "Using optimiser flag -O3 -----------------"
gcc loop_worksharing.c -fopenmp -fopt-info-vec -O3 -o loop_worksharing && ./loop_worksharing