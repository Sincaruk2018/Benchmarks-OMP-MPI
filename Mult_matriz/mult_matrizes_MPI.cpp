#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <mpi.h>
#include <filesystem>


using namespace std;

int main(int argc, char **argv)
{
	
	if(argc != 2)
	{
		std::cout << "Incorrect number of args: (ID)" << std::endl;
		return 1;
	}
	
	/* Variáveis */
	int i,j,k;
	int** matrixA, **matrixB, **matrixC;
	int num_row,num_line;
	int element, matrix_size;
	char aux = ',';
	string text_line;
	double start_time, end_time;
	double local_time, global_time;
	int row_per_process;
	int process_number;
	
	int id = atoi(argv[1]);

	
	/* Iniciando o MPI*/
	MPI_Init(&argc, &argv);
	int rank, _size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &_size);
    
    /* Alocações e preparativos prévios*/
	if(rank == 0)
	{
		ifstream file("matrices.csv");
	
		/* Erro, arquivo não existe */
		if (!file.is_open())
		{
			std::cout << "Error! File not found." << std::endl;
			MPI_Finalize();
			return 1;
		}
		

		
		
		/* Pegando as dimensões */
		getline(file,text_line);
		stringstream dims(text_line);
		dims >> num_row >> aux >> num_line;
		matrix_size = num_line * num_row;

		
		/* Mallocando */
		matrixA = (int**) malloc(num_line * sizeof(int*));
		matrixB = (int**) malloc(num_line * sizeof(int*));
		matrixC = (int**) malloc(num_line * sizeof(int*));

		
		/* Colocando o conteúdo do CSV nas células*/
		for(i = 0; i < num_line; i++) 
		{
			matrixA[i] = (int*) malloc(num_row * sizeof(int));
			matrixB[i] = (int*) malloc(num_row * sizeof(int));
			matrixC[i] = (int*) malloc(num_row * sizeof(int));
			getline(file,text_line);
			stringstream dims(text_line);
			for(j = 0; j < num_row; j++)
			{
				dims >> element;
				matrixA[i][j] = element;
				matrixB[i][j] = element;
				matrixC[i][j] = 0; /* Por garantia */
				if (j != num_row - 1)
				{
					dims >> aux;
				}
			}
		}
		file.close();
	}
	
	/* Iniciando o Broadcasting das matrizes para os outros processos*/
	MPI_Bcast(&num_row, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&num_line, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	/* Confere se a Matriz é suportada para o programa*/
	if(num_line != num_row)
	{
		if(rank == 0)
		{
			cout << "Only square matrices supported." << endl;
		}

		MPI_Finalize();
		return 1;
	}
	if(num_line % _size != 0)
	{
		if(rank == 0)
		{
			cout << "Number of rows must be divisible by process count." << endl;
		}

		MPI_Finalize();
		return 1;
	}
	
	/* Broadcast e alocação da Matriz B*/
	if(rank != 0)
	{
		matrixB = (int**) malloc(num_line * sizeof(int*));

        for(i = 0; i < num_line; i++)
        {
            matrixB[i] = (int*) malloc(num_row * sizeof(int));
        }	
	}
	for(i = 0; i < num_line; i++)
    {
		MPI_Bcast(matrixB[i], num_row, MPI_INT, 0, MPI_COMM_WORLD);
    }
    
    /* Alocando matrizes locais*/
    row_per_process = num_line/_size;    
    int** localA = (int**) malloc(row_per_process * sizeof(int*));
    int** localC = (int**) malloc(row_per_process * sizeof(int*));
	for(i = 0; i < row_per_process; i++)
	{
		localA[i] = (int*) malloc(num_row * sizeof(int));
		localC[i] = (int*) malloc(num_row * sizeof(int));
	}
	
	/* Processo mestre manda Arrays correspondentes às linhas da matriz A*/
	if (rank == 0)
	{
		for(process_number = 1; process_number < _size; process_number++)
		{
			for(i = 0; i < row_per_process; i++)
			{
				 MPI_Send(matrixA[process_number * row_per_process + i], num_row, MPI_INT, process_number, 0, MPI_COMM_WORLD);
			}
		}
		for (i = 0; i < row_per_process; i++)
		{
			for(j = 0; j < num_row; j++)
			{
				localA[i][j] = matrixA[i][j];
			}
		}
	}
	/* Processos seguidores recebem as arrays equivalentes */
	else
	{
		for(i = 0; i < row_per_process; i++)
        {
            MPI_Recv(localA[i], num_row, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
	}
	
	/* Começando o cronômetro e fazendo a multiplicação de matriz*/
    start_time = MPI_Wtime();
    for(i = 0; i < row_per_process; i++)
    {
		for(j = 0; j <num_row; j++)
		{
			localC[i][j] = 0;
			for(k = 0; k < num_row; k++)
			{
				localC[i][j] += localA[i][k] * matrixB[k][j];
			}
		}
	}
	
	/* Barreira para os processos que já terminaram a multiplicação*/
	MPI_Barrier(MPI_COMM_WORLD);
	end_time = MPI_Wtime();
	local_time = end_time - start_time;	
	MPI_Reduce(&local_time, &global_time, 1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);
	
	/* Construindo a matriz resultado e imprimindo na tela o tempo (Para fins de conferir resultado -
	 * cortada do benchmark final)*/
	/*if(rank == 0)
	{
			
			for(i = 0; i < row_per_process; i++)
			{
					for(j = 0; j < num_line; j++)
					{
						matrixC[i][j] = localC[i][j];
					}
			}
			
			for(process_number = 1; process_number < _size; process_number++)
			{
				for(i = 0; i <row_per_process; i++)
				{
					 MPI_Recv(matrixC[process_number * row_per_process + i], num_row, MPI_INT, process_number, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				}
			}
			for(i = 0; i < num_line; i++)
			{
				for(j = 0; j < num_row; j++)
				{
					cout << matrixC[i][j] << " ";
				}
				cout << endl;
			}
	}
	else
	{
		 for(i = 0; i < row_per_process; i++)
        {
            MPI_Send(localC[i], num_row, MPI_INT, 0, 1, MPI_COMM_WORLD);
        }
	}*/
	
	/* Impressão do tempo no csv resultados em um .csv*/
	if(rank == 0)
	{
		std::string filename;
		filename = "Benchmark_MultMatrices_MPI.csv";
		std::ofstream outfile(filename, std::ios::app);
		if(!outfile.is_open())
		{
			std::cout << "Couldn't open or create the file!" << std::endl;
			outfile.close();
			return 1;
		}
		if (std::filesystem::file_size(filename) == 0) 
		{
			outfile << "ID, Matrix Size, Number of Proccess, Time Elapsed(s)" << std::endl;
		}
		outfile << id << aux << matrix_size <<  aux  << _size << aux << global_time << endl;
		outfile.close();
	}
	
	
	/* Finalizando e dando free nas matrizes */
	if(rank == 0)
	{
		for(i = 0; i < num_line; i++)
		{
			free(matrixA[i]);
			free(matrixC[i]);
		}

		free(matrixA);
		free(matrixC);
	}

	for(i = 0; i < row_per_process; i++)
	{
		free(localA[i]);
		free(localC[i]);
	}
	free(localA);
	free(localC);

	for(i = 0; i < num_line; i++) 
	{
		free(matrixB[i]);
	}
	free(matrixB);
	
	MPI_Finalize();	
	return 0;
}
