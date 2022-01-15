#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include "functions.h"


int main(int argc, char** argv)
{
	printf("ARGC: %d\n", argc);
	int* niz = new int[argc - 1];

	for (int i = 1; i < argc; i++)
	{
		niz[i - 1] = atof(argv[i]);
		printf("%d ", niz[i - 1]);
	}
	//printf("\nPARAMETRI: %s", **argv);
	int matrix_size = sqrt(argc - 1);
	printf("\nDIMENZIJA MATRICE: %d", matrix_size);
	int** matrix = create_matrix(niz, matrix_size);

	delete[] niz;

	int result = calculate_determinant(matrix, matrix_size);

	printf("\nRESULT: %d", result);

	for (int i = 0; i < matrix_size; i++)
		delete[] matrix[i];
	delete[] matrix;

	ExitProcess(result);
	return 0;


}