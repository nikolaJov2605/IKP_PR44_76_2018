#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include "Queue.h"
#include "ServerPacket.h"


#pragma region Podaci
HANDLE hSemaphore;
HANDLE wSemaphore;
#pragma endregion

#pragma region Definicije
void initQueue(Queue** head);
void EnQueue(Queue** head, Server data);
Queue* DeQueue(Queue** head);
int Counter(Queue* head);
#pragma endregion

