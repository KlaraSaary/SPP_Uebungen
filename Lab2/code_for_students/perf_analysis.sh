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

nranks=32
log_file="log"
def_iters=10
warmup_iters=3
base_input=2097152
input_iters=5
input_line="POINTS "

# Timing should be initialized each time before the iterations start
rm -f .cpu_freq

j=1
while [ $j -le ${input_iters} ]
do
	input=`echo "${base_input} * ${j}" | bc -l`
	input_line="${input_line} ( $input )"
	
	i=1
	iters=$warmup_iters             # Warmup iters
	while [ $i -le $iters ]
	do
		mpirun -n $nranks ./task2 $input > /dev/null

		i=$(( i + 1 ))
	done

	data_line="DATA"
	i=1
	iters=$def_iters                # Normal iters
	while [ $i -le $iters ]
	do
		mpirun -n $nranks ./task2 $input > log_${j}_${i}
		exect=`cat "log_${j}_${i}" | grep 'Elapsed' | awk '{print $4}'`
		data_line="${data_line} $exect"
		
		i=$(( i + 1 ))
	done
	echo ${data_line} >> input.tmp
	
	rm -f log_*
	j=$(( j + 1 ))
	
	echo "Finished input ${input}..."
done

# Create the Extra-P input file
echo "X p" > input.res
echo "${input_line}" >> input.res
echo "REGION reg" >> input.res
echo "METRIC exect" >> input.res
cat input.tmp >> input.res
rm -f input.tmp

echo "DONE!"
