#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <omp.h>
#include <chrono>
#include <filesystem>


using namespace std;

int main(int argc, char **argv)
{
	if(argc != 3)
	{
		std::cout << "Incorrect number of args: (ID), Number of Threads" << std::endl;
		return 1;
	}
	
	/* Variáveis */
	int i,j,k;
	int** matrixA, **matrixB, **matrixC;
	int num_row,num_line;
	int matrix_size, num_threads;
	int element, id;
	char aux = ',';
	
	string text_line;
	ifstream file("matrices.csv");
	
	/* Erro, arquivo não existe */
	if (!file.is_open())
	{
		std::cout << "Error! File not found." << std::endl;
		return 1;
	}
	
	/* Preparando o arquivo */
	std::string filename;
	filename = "Benchmark_MultMatrices_OMP.csv";
	std::ofstream outfile(filename, std::ios::app);
	
	if(!outfile.is_open())
	{
		std::cout << "Couldn't open or create the file!" << std::endl;
		outfile.close();
		return 1;
	}
	if (std::filesystem::file_size(filename) == 0) 
	{
		outfile << "ID, Matrix Size, Threads, Time Elapsed(s)" << std::endl;
	}
	
	id = atoi(argv[1]);
	num_threads = atoi(argv[2]);
	omp_set_num_threads(num_threads);

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
	
	
	
	auto start = chrono::high_resolution_clock::now();

	/* Multiplicação de Matrizes */
	#pragma omp parallel for private(j,k)
	for(i = 0; i < num_line; i++) 
    {
		for(j = 0; j < num_row; j++)
		{
			for(k = 0; k < num_row; k++)
			{
				matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
			}
		}
	}
			
	/* Fim da cronometragem*/
	auto end = chrono::high_resolution_clock::now();
	chrono::duration<double> elapsed = end - start;
    /*cout << "Time: " << elapsed.count() << " seconds" << endl;*/
	outfile << id << aux << matrix_size <<  aux << num_threads << aux << elapsed << endl;

	
	/* Closing */
	for(i = 0; i < num_line; i++) 
	{
		free(matrixA[i]);
		free(matrixB[i]);
		free(matrixC[i]);
	}
	free(matrixA);
	free(matrixB);
	free(matrixC);
    file.close();
	
	return 0;
}
