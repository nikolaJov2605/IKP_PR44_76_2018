#include "pch.h"
#include "functions.h"

char* FullPath(char* relativePath)
{
	char* full = new char[_MAX_PATH];
	if (_fullpath(full, relativePath, _MAX_PATH) != NULL)
	{
		return full;
	}
	else
	{
		printf("Invalid path\n");
		return 0;
	}
}

#pragma region Run_Another_Process

int run_process(char** arguments)
{
	char relativePath[24] = "..\\Debug\\Workers.exe";
	char* full_path = FullPath(relativePath);
	int res = _spawnv(P_WAIT, full_path, arguments);

	delete[] full_path;
	return res;
}