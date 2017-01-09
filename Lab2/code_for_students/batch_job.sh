#!/bin/bash
#SBATCH -J par-sorting
#SBATCH -e log.err.%j
#SBATCH -o log.out.%j
#SBATCH -n 32                  # Number of procs
#SBATCH -c 1				   # 1 thread per process
#SBATCH --mem-per-cpu=1800     # Main memory in MByte for each cpu core
#SBATCH --nodes=2
#SBATCH -t 00:05:00            # Hours and minutes, or '#SBATCH -t 10' - only minutes
#SBATCH --exclusive
#SBATCH -p kurs1

# -------------------------------
# Afterwards you write your own commands, e.g.
module load gcc openmpi/gcc
make all
mpirun -n 32 ./task2 32768
