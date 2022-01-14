#pragma once

#include "functions.h"\

int SendResultToClient(char* data, int num, SOCKET clientSocket) {
	SOCKET connectSocket = clientSocket;

	int iResult;

	if (connectSocket == INVALID_SOCKET)
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	iResult = send(connectSocket, (char*)data, num, 0);

	if (iResult == SOCKET_ERROR)
	{
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(connectSocket);
		WSACleanup();
		return 1;
	}

	return 0;
}