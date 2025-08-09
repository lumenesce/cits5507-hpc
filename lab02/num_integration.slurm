#!/bin/bash

#SBATCH --job-name=num_integration
#SBATCH --output=num_integration.out
#SBATCH --error=num_integration.err

#SBATCH --cpus-per-task=4
#SBATCH --time=00:00:30
#SBATCH --mem=1G
#SBATCH --partition=cits3402

echo "Using optimiser flag -O3 -----------------"
gcc num_integration.c -fopenmp -fopt-info-vec -O3 -o num_integration && ./num_integration