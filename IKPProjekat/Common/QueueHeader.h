#pragma once

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "conio.h"

typedef struct ClientMessage_st {
	int size;
	char* data;
}Client;

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

void initQueue(Queue** head);
void EnQueue(Queue** head, Server data);
Queue* DeQueue(Queue** head);
int NumberOfElements(Queue* head);