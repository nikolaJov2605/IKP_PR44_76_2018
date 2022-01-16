
#pragma warning(disable : 4996)
#include "functions.h"
#include "constants.h"
#include "../Common/functions.cpp"

// kreira se i ispisuje matrica na osnovu niza i dimenzije
int** create_matrix(int* niz, int size)
{
	int** matrix = new int* [size];
	for (int i = 0; i < size; i++)
		matrix[i] = new int[size];
	int br = 0;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			matrix[i][j] = niz[br];
			br++;
		}
	}
	printf("\n");
	printf("\n");
	for (int i = 0; i < size; i++)
	{
		printf("\n");
		for (int j = 0; j < size; j++)
		{
			printf("%d ", matrix[i][j]);
		}
	}

	// dealocirati pokazivac
	return matrix;

}
// odredjivanje podmatrice za trenutnu vrstu i kolonu
void get_submatrix(int** matrix, int** temp_matrix, int row, int collumn, int matrix_size)
{
	// indeksi vrsta i kolona podmatrice
	int k = 0;
	int p = 0;
	for (int i = 0; i < matrix_size; i++)
	{
		for (int j = 0; j < matrix_size; j++)
		{
			// iteriramo kroz matricu radeci dekompoziciju po row vrsti i collumn koloni, tako da te vrste i kolone ignorisemo
			if (i != row && j != collumn)
			{
				temp_matrix[k][p] = matrix[i][j];
				p++;

				if (p == matrix_size - 1)
				{
					p = 0;
					k++;
				}
			}
		}
	}
}

// rekurzivna metoda koja racuna determinantu
int calculate_determinant(int** matrix, int matrix_size)
{
	int result = 0;

	int** temp_matrix;

	int znak = 1;

	//ako je matrica 1x1, vracamo njen jedini element
	if (matrix_size == 1)
	{
		result = matrix[0][0];
		return result;
	}
	// ako je matrica 2x2, racunamo po standardnoj formuli za taj slucaj
	if (matrix_size == 2)
	{
		result = matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1];
		return result;
	}

	// ako je matrica 3x3, dodajemo poslednje dve kolone na kraj i resavamo je Sarusovim pravilom
	if (matrix_size == 3)
	{
		result = matrix[0][0] * matrix[1][1] * matrix[2][2] + matrix[0][1] * matrix[1][2] * matrix[2][0] + matrix[0][2] * matrix[1][0] * matrix[2][1]
			- matrix[2][0] * matrix[1][1] * matrix[0][2] - matrix[2][1] * matrix[1][2] * matrix[0][0] - matrix[2][2] * matrix[1][0] * matrix[0][1];

		return result;
	}

	// ako je matrica veca, razbijamo je rekurzivno
	int m_size = pow(matrix_size - 1, 2);

	temp_matrix = new int* [matrix_size - 1];
	for (int i = 0; i < matrix_size - 1; i++)
	{
		temp_matrix[i] = new int[matrix_size - 1];
	}

	for (int i = 0; i < matrix_size; i++)
	{
		get_submatrix(matrix, temp_matrix, 0, i, matrix_size);

		char** arguments = new char* [m_size + 2];
		for (int i = 0; i < m_size + 2; i++)
		{
			arguments[i] = new char[20];
		}

		strcpy_s(arguments[0], 20, "Workers.exe");

		int* parameters = new int[m_size];
		int idx = 0;
		for (int i = 0; i < matrix_size - 1; i++)
		{
			for (int j = 0; j < matrix_size - 1; j++)
			{
				parameters[idx] = temp_matrix[i][j];
				idx++;
			}
		}

		for (int i = 0; i < m_size + 1; i++)
		{
			_itoa(parameters[i], arguments[i + 1], 10);
		}
		
		arguments[m_size + 1] = NULL;
		delete[] parameters;

		//result += znak * matrix[0][i] * calculate_determinant(temp_matrix, matrix_size - 1);
		result += znak * matrix[0][i] * run_process(arguments, matrix_size - 1);
		printf("\nRESULT: %d", result);

		znak = -znak;

	}
	
	

	for (int i = 0; i < matrix_size - 1; i++) {
		delete[] temp_matrix[i];
		temp_matrix[i] = NULL;
	}
	delete[] temp_matrix;
	temp_matrix = NULL;
	return result;

}