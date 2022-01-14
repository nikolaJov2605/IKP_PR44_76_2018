#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <math.h>
#include <time.h>
#include "conio.h"



void menu();
char* input_matrix(int size);
char* generate_random_matrix(int size);
int run_stres_test(SOCKET connectSocket);
void close_Server(SOCKET socket);
