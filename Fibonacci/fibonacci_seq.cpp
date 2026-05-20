#include <iostream>
#include <cstdlib>
#include <chrono>
#include <fstream>
#include <filesystem>

using namespace std;

long long fibonacci(int n)
{
	if (n <= 2)
	{
		return n;
	}
	return fibonacci(n-1) + fibonacci(n-2);
}

int main(int argc, char **argv)
{
	if(argc != 3)
	{
		std::cout << "Incorrect number of args: (id, nth fibonacci term)" << std::endl;
		return 1;
	}
	
	/* Preparando o arquivo */
	std::string filename;
	filename = "Benchmark_Fibonacci_Sequential.csv";
	std::ofstream outfile(filename, std::ios::app);

	/* Preparativos de argumento e arquivo */

	if(!outfile.is_open())
	{
		std::cout << "Couldn't open or create the file!" << std::endl;
		outfile.close();
		return 1;
	}
	if (std::filesystem::file_size(filename) == 0) 
	{
		outfile << "ID, Fibonacci Term, Time Elapsed(s)" << std::endl;
	}
	
	/* Variáveis de suporte */
	char comma = ',';
	int id_exp = atoi(argv[1]);
	int n_term = atoi(argv[2]);

	/* Realizando a fibonnaci */
	auto start = chrono::high_resolution_clock::now();
	long long result = fibonacci(n_term);
	printf("%lld\n",result); /* Só pra evitar warning */
	auto end = chrono::high_resolution_clock::now();
	chrono::duration<double> elapsed = end - start;
    /*cout << "Result: " << result << endl;
    cout << "Time: " << elapsed.count() << " seconds" << endl;*/
    
    /* Colocando tudo no arquivo*/
    outfile << id_exp << comma << n_term << comma << elapsed << std::endl;
    outfile.close();
    
	
	
	return 0;
}
