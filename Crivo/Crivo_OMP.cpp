
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <omp.h>
#include <filesystem>


#define UNMARKED 0
#define MARKED 1



int main(int argc, char **argv)
{
	
	/* Declarando arquivo */
	std::string filename;
	filename = "Benchmark_Crive_OMP.csv";
	std::ofstream outfile(filename, std::ios::app);
	
	/* Preparando o arquivo*/
	if(argc != 4)
	{
		std::cout << "Needs 3 arguments: ID, Crive Size and Number of threads" << std::endl;
		return 1;
	}
	if(!outfile.is_open())
	{
		std::cout << "Couldn't open or create the file!" << std::endl;
		outfile.close();
		return 1;
	}
	if (std::filesystem::file_size(filename) == 0) 
	{
		outfile << "ID, Crive Size, Threads, Time Elapsed(s)" << std::endl;
	}
	
	omp_set_num_threads(atoi(argv[2]));

	
	/* Declarando variáveis */
	int arr_size;
	int* arr_crive;
	int i, j;
	char comma = ',';
	int num_threads = atoi(argv[3]);
	int num_exp = atoi(argv[1]);
	
	
	
	/* Setting up the main array*/
	arr_size = atoi(argv[2]);
	arr_crive = (int*) malloc((arr_size + 1) * sizeof(int));
	arr_crive[0] = MARKED;
	arr_crive[1] = MARKED;
	
	for (i = 2; i <= arr_size; i++)
	{
		arr_crive[i] = UNMARKED;
	}
	
	auto start_ = std::chrono::high_resolution_clock::now();
	#pragma omp parallel
	{
		for (int i = 2; i * i <= arr_size; i++)
		{
			if (arr_crive[i] == UNMARKED)
			{
				#pragma omp for schedule(static)
				for (int j = i * i; j <= arr_size; j += i)
				{
					arr_crive[j] = MARKED;
				}
			}
		}
	}
	auto stop_ = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = stop_ - start_;

	
	/*for (i = 2; i < arr_size; i++)
	{
		if(arr_crive[i] == UNMARKED)
		{
			results_prime << i << std::endl;
		}
	}*/
	
	/* Colocando os dados no arquivo */
	outfile << num_exp << comma << arr_size << comma << num_threads << comma << elapsed << std::endl;


	/* Finished */
	free(arr_crive);
	outfile.close();		
	return 0;
}

