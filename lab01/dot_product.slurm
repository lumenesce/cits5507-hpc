#!/bin/bash

#SBATCH --job-name=dot_product
#SBATCH --output=dot_product.out
#SBATCH --error=dot_product.err

#SBATCH --cpus-per-task=4
#SBATCH --time=00:00:30
#SBATCH --mem=1G
#SBATCH --partition=cits3402

echo "✅ Using optimiser flag -00 -----------------"
gcc dot_product.c -fopt-info-vec -O0 -o dot_O0 && ./dot_O0

echo "\n✅ Using optimiser flag -01 -----------------"
gcc dot_product.c -fopt-info-vec -O1 -o dot_O1 && ./dot_O1

echo "\n✅ Using optimiser flag -02 -----------------"
gcc dot_product.c -fopt-info-vec -O2 -o dot_O2 && ./dot_O2

echo "\n✅ Using optimiser flag -03 -----------------"
gcc dot_product.c -fopt-info-vec -O3 -o dot_O3 && ./dot_O3