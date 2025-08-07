#!/bin/bash

#SBATCH --cpus-per-task=4
#SBATCH --time=00:05:00
#SBATCH --mem=1G
#SBATCH --partition=cits3402

gcc -O3 -mavx512f -o dot_intrinsics dot_product_intrinsics.c && ./dot_intrinsics