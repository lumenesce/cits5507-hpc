#!/bin/bash

#SBATCH --cpus-per-task=4
#SBATCH --time=00:05:00
#SBATCH --mem=1G
#SBATCH --partition=cits3402

gcc dot_product.c -fopt-info-vec -O0 -o dot_O0 && ./dot_O0

gcc dot_product.c -fopt-info-vec -O3 -o dot_O3 && ./dot_O3