#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include "../Common/Queue.cpp"


#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define SERVER_IP_ADDRESS "127.0.0.1"
#define SERVER_PORT 27017
#define BUFFER_SIZE 256
#define MAX_CLIENTS 5


int run_process(char* parameters);

int** create_matrix(int* data, int size);
void get_submatrix(int** matrix, int** temp_matrix, int row, int collumn, int matrix_size);
int calculate_determinant(int** matrix, int matrix_size);

int main(int argc, char** argv)
{
	int* niz = new int[argc - 1];
	
	for (int i = 1; i < argc - 1; i++)
	{
		niz[i - 1] = atof(argv[i]);
		printf("%d ", niz[i - 1]);
	}
	//printf("\nPARAMETRI: %s", **argv);
	int matrix_size = sqrt(argc - 2);
	printf("\nDIMENZIJA MATRICE: %d", matrix_size);

	int** matrix = create_matrix(niz, matrix_size);

	delete[] niz;

	int result = calculate_determinant(matrix, matrix_size);

	printf("\nRESULT: %d", result);
	for (int i = 0; i < matrix_size; i++)
		delete[] matrix[i];
	delete[] matrix;

	//_getch();
	ExitProcess(result);
	return 0;


}

int run_process(char* parameters)
{
	// da bi struktura primila parametre za poziv procesa, potrebno ih je konvertovati u odgovarajuci tip
	const WCHAR* params; //LPCWSTR

	//int size = MultiByteToWideChar(CP_ACP, 0, parameters, -1, NULL, 0);

	int s = strlen(parameters);

	params = new WCHAR[s];
	MultiByteToWideChar(CP_ACP, 0, parameters, -1, (LPWSTR)params, s);



	// kreiramo i popunjavamo strukturu procesa
	SHELLEXECUTEINFO ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = L"open";
	ShExecInfo.lpFile = L"C:\\Users\\TUF\\Desktop\\ProjektiSemestar1\\IKPProjekat\\IKP_PR44_76_2018\\IKPProjekat\\Debug\\Workers.exe";
	ShExecInfo.lpParameters = params;
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = NULL;
	if (ShellExecuteExW(&ShExecInfo) == false)
	{
		printf("\nError %d\n", WSAGetLastError());
		return WSAGetLastError();
	}

	delete[] params;

	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

	DWORD output = -1;
	bool success = GetExitCodeProcess(ShExecInfo.hProcess, &output);
	if (!success) {
		printf("\nProces exit code error: %d", GetLastError());
	}

	CloseHandle(ShExecInfo.hProcess);

	//ExitProcess(-1);
	//exit(output);

	return output;
}

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

	return matrix;

}


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

int calculate_determinant(int** matrix, int matrix_size)
{
	int result = 0;

	int** temp_matrix;

	int znak = 1;

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

	temp_matrix = new int* [matrix_size - 1];
	for (int i = 0; i < matrix_size - 1; i++)
	{
		temp_matrix[i] = new int[matrix_size - 1];
	}

	for (int i = 0; i < matrix_size; i++)
	{
		get_submatrix(matrix, temp_matrix, 0, i, matrix_size);

		char params[2048];
		int* parameters = new int[(matrix_size - 1) * (matrix_size - 1)];
		int idx = 0;
		for (int i = 0; i < matrix_size - 1; i++)
		{
			for (int j = 0; j < matrix_size - 1; j++)
			{
				parameters[idx] = temp_matrix[i][j];
				idx++;
			}
		}
		int len = 0;
		for (int i = 0; i < (matrix_size - 1) * (matrix_size - 1); i++)
		{
			len += snprintf(params + len, 2048 - len, "%d ", parameters[i]);
		}
		//printf("%s", params);

		delete[] parameters;

		//result += znak * matrix[0][i] * calculate_determinant(temp_matrix, matrix_size - 1);
		result += znak * matrix[0][i] * run_process(params);
		printf("\nRESULT: %d", result);

		znak = -znak;
	}

	for (int i = 0; i < matrix_size - 1; i++)
		delete[] temp_matrix[i];
	delete[] temp_matrix;

	return result;

}