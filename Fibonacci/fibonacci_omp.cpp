#include <iostream>
#include <cstdlib>
#include <chrono>
#include <fstream>
#include <omp.h>
#include <filesystem>

using namespace std;

long long fibonacci(int n)
{
	if (n <= 2)
	{
		return n;
	}
	
	long long x,y;

	#pragma omp task shared(x)
    x = fibonacci(n - 1);

    #pragma omp task shared(y)
    y = fibonacci(n - 2);
	#pragma omp taskwait
	return x+y;
}

int main(int argc, char **argv)
{
	

	/* Preparativos de argumento e arquivo */
	if(argc != 4)
	{
		std::cout << "Incorrect number of args: (id, nth fibonacci term, threads)" << std::endl;
		return 1;
	}
	/* Preparando o arquivo */
	std::string filename;
	filename = "Benchmark_Fibonacci_OMP.csv";
	std::ofstream outfile(filename, std::ios::app);
	if(!outfile.is_open())
	{
		std::cout << "Couldn't open or create the file!" << std::endl;
		outfile.close();
		return 1;
	}
	if (std::filesystem::file_size(filename) == 0) 
	{
		outfile << "ID, Fibonacci Term, Num of Threads, Time Elapsed(s)" << std::endl;
	}

	/* Variáveis de suporte */
	char comma = ',';
	int id_exp = atoi(argv[1]);
	int n_term = atoi(argv[2]);
	int n_threads = atoi(argv[3]);
	int result;

	/* Configurando o número de threads usadas*/
	omp_set_num_threads(n_threads);
	
	/* Realizando a fibonnaci */
	auto start = chrono::high_resolution_clock::now();
	#pragma omp parallel
    {
        #pragma omp single
        {
            result = fibonacci(n_term);
        }
    }
	printf("%lld\n",result);
	auto end = chrono::high_resolution_clock::now();
	chrono::duration<double> elapsed = end - start;

    /*cout << "Result: " << result << endl;
    cout << "Time: " << elapsed.count() << " seconds" << endl;*/
    
	/* Colocando tudo no arquivo */
    outfile << id_exp << comma << n_term << comma << n_threads << comma << elapsed << std::endl;
    outfile.close();
    
	
	
	return 0;
}
