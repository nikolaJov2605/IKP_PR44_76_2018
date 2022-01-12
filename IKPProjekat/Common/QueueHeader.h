#pragma once

#include <windows.h>
#pragma comment(lib, "SHELL32.LIB")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "conio.h"
#include <math.h>
#include <shellapi.h>
#include <time.h>


#pragma region Podaci
HANDLE hSemaphore;
HANDLE wSemaphore;

typedef struct ServerPacket_st {
	char* data;
	SOCKET socket;
}Server;

typedef struct Queue {
	char* data;
	SOCKET socket;
	struct Queue* next;
}Queue;
#pragma endregion



#pragma region Definicije
void initQueue(Queue** head);
void EnQueue(Queue** head, Server data);
Queue* DeQueue(Queue** head);
int NumberOfElements(Queue* head);
#pragma endregion

