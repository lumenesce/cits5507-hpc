#!/bin/bash

#SBATCH --job-name=matrix_mult
#SBATCH --output=matrix_mult.out
#SBATCH --error=matrix_mult.err

#SBATCH --cpus-per-task=8
#SBATCH --time=00:00:30
#SBATCH --mem=1G
#SBATCH --partition=cits3402

echo "Executed using optimiser flag -O3 -----------------\n"
gcc matrix_mult.c -fopenmp -fopt-info-vec -O3 -o matrix_mult && ./matrix_mult