
#!/bin/bash
set -e

cd Mult_matriz || exit
payloads=(4 32 128 1024 1536)

count_seq=0
count_par=0

rm -f Benchmark_*.csv
echo "Running Matrix..."
for i in {1..10}
do
	echo "$i/10"
	for payload in "${payloads[@]}"
	do
		echo "Payload: $payload"
		echo "Sequential"
		./geramatrix.out "$payload"
		./mult_matriz_seq.out "$count_seq"
		((++count_seq))
		echo "Sequential Done!"
		
		echo "OMP 2 threads"
		./mult_matriz_omp.out "$count_par" 2
		echo "Done!"
		echo "MPI 1 Core"
		mpiexec -np 1 ./mult_matriz_mpi.out "$count_par"
		((++count_par))
		echo "Done!"
		
		echo "OMP 4 threads"
		./mult_matriz_omp.out $count_par 4
		echo "Done!"
		echo "MPI 2 Core"
		mpiexec -np 2 ./mult_matriz_mpi.out "$count_par"
		echo "Done!"
		((++count_par))

		
		echo "OMP 8 threads"
		./mult_matriz_omp.out $count_par 8
		echo "Done!"
		echo "MPI 4 Core"
		mpiexec -np 4 ./mult_matriz_mpi.out "$count_par"
		echo "Done!"
		((++count_par))
	done
done

cd ..



