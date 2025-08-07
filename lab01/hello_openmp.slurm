#!/bin/bash

#SBATCH --cpus-per-task=4
#SBATCH --time=00:05:00
#SBATCH --mem=1G
#SBATCH --partition=cits3402

gcc -fopenmp hello_openmp.c -o hello_openmp
./hello_openmp