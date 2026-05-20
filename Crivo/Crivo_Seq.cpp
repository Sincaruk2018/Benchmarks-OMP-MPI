
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <filesystem>

#define UNMARKED 0
#define MARKED 1




int main(int argc, char **argv)
{
	/* Declarando variáveis */
	std::string filename;
	filename = "Benchmark_Crive_Sequential.csv";
	std::ofstream outfile(filename, std::ios::app);
	
	int arr_size;
	int* arr_crive;
	int id_exp = atoi(argv[1]);
	int i, j;
	char comma = ',';
	
	/* Preparando o arquivo*/
	if(argc != 3)
	{
		std::cout << "Needs 2 arguments: ID, Crive Size" << std::endl;
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
		outfile << "ID, Crive Size, Time Elapsed(s)" << std::endl;
	}
	
	
	arr_size = atoi(argv[2]);
	arr_crive = (int*) malloc((arr_size + 1) * sizeof(int));
	arr_crive[0] = MARKED;
	arr_crive[1] = MARKED;

	for (i = 2; i <= arr_size; i++)
	{
		arr_crive[i] = UNMARKED;
	}
	
	auto start_ = std::chrono::high_resolution_clock::now();
	for (i = 2; i*i <= arr_size; i++)
	{
		if(arr_crive[i] == UNMARKED)
		{
			for(j = i*2; j < arr_size+1; j += i)
			{
				arr_crive[j] = MARKED;
			}
		}
	}
	auto stop_ = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = stop_ - start_;

	/*for (i = 2; i < arr_size; i++)
	{
		if(arr_crive[i] == UNMARKED)
		{
			std::cout << i << std::endl;
			results_prime << i << std::endl;
		}
	}*/
	
	outfile << id_exp << comma << arr_size << comma << elapsed << std::endl;
	free(arr_crive);
	outfile.close();
	return 0;
}

