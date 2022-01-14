#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#pragma comment(lib, "SHELL32.LIB")
#include <shellapi.h>

char* FullPath(char* partialPath);
int run_process(char* parameters);