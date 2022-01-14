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

int run_process(char* parameters)
{
	// da bi struktura primila parametre za poziv procesa, potrebno ih je konvertovati u odgovarajuci tip
	const WCHAR* params; //LPCWSTR
	WCHAR* path;

	//int size = MultiByteToWideChar(CP_ACP, 0, parameters, -1, NULL, 0);

	int s = strlen(parameters);

	params = new WCHAR[s];
	MultiByteToWideChar(CP_ACP, 0, parameters, -1, (LPWSTR)params, s);

	char relativePath[24] = "..\\Debug\\Workers.exe";
	char* full_path = FullPath(relativePath);
	s = strlen(full_path);

	path = new WCHAR[s];
	MultiByteToWideChar(CP_ACP, 0, full_path, -1, (LPWSTR)path, s);

	path[s] = L'\0';


	// kreiramo i popunjavamo strukturu procesa
	SHELLEXECUTEINFO ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = L"open";
	ShExecInfo.lpFile = path;
	ShExecInfo.lpParameters = params;
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = NULL;
	if (ShellExecuteExW(&ShExecInfo) == false)
	{
		printf("\nError %d\n", GetLastError());
		return GetLastError();
	}

	delete[] params;
	delete[] full_path;


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