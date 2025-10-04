#!/bin/bash

#SBATCH --job-name=critical_atomic
#SBATCH --output=critical_atomic.out
#SBATCH --error=critical_atomic.err

#SBATCH --cpus-per-task=4
#SBATCH --time=00:00:30
#SBATCH --mem=1G
#SBATCH --partition=cits3402

echo "Executed using optimiser flag -O3 -----------------\n"
gcc critical_atomic.c -fopenmp -fopt-info-vec -O3 -o critical_atomic && ./critical_atomic