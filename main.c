#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "simplex.c"

void print_matrix(double **matrix, int row_size, int column_size);

int main(int argc, char* argv[])
{
	srand(time(NULL));
	double **matrix = calloc(4, sizeof(double));
	for (int i = 0; i < 4; i++)
	{
		matrix[i] = calloc(7, sizeof(double));
	}
	/*for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			matrix[i][j] = rand() % 10 + 1;
		}
	}*/

	//fabrica de puertas y ventanas example test
	matrix[0][0] = 1;
	matrix[0][1] = -3;
	matrix[0][2] = -5;
	matrix[1][1] = 1;
	matrix[1][3] = 1;
	matrix[1][6] = 4;
	matrix[2][2] = 2;
	matrix[2][4] = 1;
	matrix[2][6] = 12;
	matrix[3][1] = 3;
	matrix[3][2] = 2;
	matrix[3][5] = 1;
	matrix[3][6] = 18;
	print_matrix(matrix, 4, 7);
	printf("------------------------------------------\n");

	maximize_algorithm(&matrix, 7, 4);

	print_matrix(matrix, 4, 7);
	return 0;
}

void print_matrix(double **matrix, int row_size, int column_size)
{
	for (int i = 0; i < row_size; i++)
	{
		for (int j = 0; j < column_size; j++)
		{
			printf("%.2f\t", matrix[i][j]);
		}
		printf("\n");
	}
}
