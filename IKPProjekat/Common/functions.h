#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>

char* FullPath(char* partialPath);
int run_process(char** arguments, int matrix_size);