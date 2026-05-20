#!/bin/bash
set -e

cd Crivo || exit
payloads=(4 128 1024 4096 16384)

count_seq=0
count_par=0

rm -f Benchmark_*.csv
echo "Running Crives..."
for i in {1..10}
do	
	echo "$i/10"
	for payload in "${payloads[@]}"
	do
		echo "Payload: $payload"
		echo "Sequential"
		./crivo_seq.out "$count_seq" "$payload"
		((++count_seq))
		echo "Sequential Done!"

		echo "OMP 2 threads"
		./crivo_omp.out "$count_par" "$payload" 2
		echo "Done!"
		echo "MPI 1 Core"
		mpiexec -np 1 ./crivo_mpi.out "$count_par" "$payload"
		((++count_par))
		echo "Done!"

		echo "OMP 4 threads"
		./crivo_omp.out "$count_par" "$payload" 4
		echo "Done!"
		echo "MPI 2 Core"
		mpiexec -np 2 ./crivo_mpi.out "$count_par" "$payload"
		((++count_par))
		echo "Done!"
		
		echo "OMP 8 threads"
		./crivo_omp.out "$count_par" "$payload" 8
		echo "Done!"
		echo "MPI 4 Core"
		mpiexec -np 4 ./crivo_mpi.out "$count_par" "$payload"
		((++count_par))
		echo "Done!"

		
	
	done
done

cd ..



