#pragma once
#pragma warning(disable : 4996)

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <math.h>



int** create_matrix(int* data, int size);
void get_submatrix(int** matrix, int** temp_matrix, int row, int collumn, int matrix_size);
int calculate_determinant(int** matrix, int matrix_size);