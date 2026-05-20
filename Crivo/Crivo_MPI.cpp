
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <mpi.h>
#include <iomanip>
#include <filesystem>

#define UNMARKED 0
#define MARKED 1



int main(int argc, char **argv)
{
	
    /* Preparando o arquivo*/
	if(argc != 3)
	{
		std::cout << "Needs 2 arguments: ID, Crive Size" << std::endl;
		return 1;
	}
	
	/* Declarando Variáveis*/
	int arr_size;
	int* arr_crive;
	int i, j, aux;
	int rank, _size;
	int begin_process, end_process, interval, local_size, limit;
	int run_id = atoi(argv[1]);
	char comma = ',';
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &_size);
    
	
	arr_size = atoi(argv[2]);
	limit = sqrt(arr_size);
	interval = arr_size/_size;
	begin_process = rank * interval;
	int* base_primes = (int*) malloc((limit + 1) * sizeof(int));
	
	
	if(rank == _size - 1)
	{
		end_process = arr_size;
	}
	else
	{
		end_process = begin_process + interval - 1;
	}
	
	local_size = end_process - begin_process + 1;
	arr_crive = (int*) malloc(local_size * sizeof(int));

	
	for (i = 0; i < local_size; i++)
	{
		arr_crive[i] = UNMARKED;
	}
	for(i = 0; i <= limit; i++)
    {
        base_primes[i] = UNMARKED;
    }

	base_primes[0] = MARKED;
    base_primes[1] = MARKED;
	
	double start_time = MPI_Wtime();
	
	for (i = 2; i*i <= limit; i++)
	{
		if(base_primes[i] == UNMARKED)
		{
			for(j = i*2; j <= limit; j += i)
			{
				base_primes[j] = MARKED;
			}
		}
	}
	
	for (i = 2; i <= limit; i++)
{
    if(base_primes[i] == UNMARKED)
    {
        if(begin_process % i == 0)
        {
            aux = begin_process;
        }
        else
        {
            aux =
                begin_process +
                (i - begin_process % i);
        }

        if(aux < i * 2)
        {
            aux = i * 2;
        }

        for(j = aux;
            j <= end_process;
            j += i)
        {
            arr_crive[
                j - begin_process
            ] = MARKED;
        }
    }
}
	MPI_Barrier(MPI_COMM_WORLD);
	double end_time = MPI_Wtime();
	/*for(i = 0; i < local_size; i++)
    {
        int number = begin_process + i;

        if(number >= 2 && arr_crive[i] == UNMARKED)
        {
            std::cout << number << std::endl;
        }
    }*/
    
    double local_time = end_time - start_time;
	double global_time;
	MPI_Reduce(&local_time, &global_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	
	/* Escrevendo*/
	 if(rank == 0)
    {
		std::string filename;
		filename = "Benchmark_Crive_MPI.csv";
		std::ofstream outfile(filename, std::ios::app);
		if(!outfile.is_open())
		{
			std::cout << "Couldn't open or create the file!" << std::endl;
			outfile.close();
			return 1;
		}
		if (std::filesystem::file_size(filename) == 0) 
		{
			outfile << "ID, Crive Size, Number of Proccess, Time Elapsed(s)" << std::endl;
		}
		outfile << run_id << comma << arr_size <<  comma  << _size << comma << global_time << std::endl;
		outfile.close();
    }
	
	/* Encerrando... */
	free(base_primes);
	free(arr_crive);
	MPI_Finalize();
	return 0;
}

