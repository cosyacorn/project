#!/bin/sh
#SBATCH -n 16
#SBATCH -t 0-01:00:00
#SBATCH -p compute
#SBATCH -J sharding_ising

module load default-intel-openmpi

mpirun -n 16 ./ring

