
#!/bin/bash
set -e

cd Fibonacci || exit
payloads=(4 8 16 32 36)

count_seq=0
count_par=0

rm -f Benchmark_*.csv
echo "Running Fibonnaci..."
for i in {1..10}
do
	echo "$i/10"
	for payload in "${payloads[@]}"
	do
		echo "Payload: $payload"
		echo "Sequential"
		./fibonacci_seq.out "$count_seq" "$payload"
		((++count_seq))
		echo "Sequential Done!"

		echo "OMP 2 threads"
		./fibonacci_omp.out "$count_par" "$payload" 2
		((++count_par))
		
		echo "OMP 4 threads"
		./fibonacci_omp.out "$count_par" "$payload" 4
		((++count_par))	
			
		echo "OMP 8 threads"
		./fibonacci_omp.out "$count_par" "$payload" 8
		((++count_par))

	done
done

cd ..



