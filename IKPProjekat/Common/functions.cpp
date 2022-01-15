#include "pch.h"
#include "functions.h"

char* FullPath(char* relativePath)
{
	char* full = new char[MAX_PATH];
	if (_fullpath(full, relativePath, MAX_PATH) != NULL)
	{
		return full;
	}
	else
	{
		printf("Invalid path\n");
		delete[] full;
		return 0;
	}
}

#pragma region Run_Another_Process

int run_process(char** arguments, int matrix_size)
{
	int res = 0;
	char relativePath[24] = "..\\Debug\\Workers.exe";
	char* full_path = FullPath(relativePath);
	res = _spawnv(P_WAIT, full_path, arguments);
	for (int i = 0; i < matrix_size * matrix_size + 2; i++)
	{
		//memset(arguments[i], 0, strlen(arguments[i]));
		delete[] arguments[i];
	}
	delete[] arguments;
	delete[] full_path;
	return res;
}