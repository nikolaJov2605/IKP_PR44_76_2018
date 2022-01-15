#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "functions.h"
#include "constants.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#pragma pack(1)

// TCP client that use non-blocking sockets
int main()
{
	srand(time(0));
#pragma region TCP
	// Socket used to communicate with server
	SOCKET connectSocket = INVALID_SOCKET;


	// Variable used to store function return value
	int iResult;

	// Buffer we will use to store message
	char* dataBuffer = new char[BUFFER_SIZE];
	bool valid = false;

	// WSADATA data structure that is to receive details of the Windows Sockets implementation
	WSADATA wsaData;

	// Initialize windows sockets library for this process
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartup failed with error: %d\n", WSAGetLastError());
		return 1;
	}

	// create a socket
	connectSocket = socket(AF_INET,
		SOCK_STREAM,
		IPPROTO_TCP);


	if (connectSocket == INVALID_SOCKET)
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	// Create and initialize address structure
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;								// IPv4 protocol
	serverAddress.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);	// ip address of server
	serverAddress.sin_port = htons(SERVER_PORT);					// server port




	// Connect to server specified in serverAddress and socket connectSocket
	iResult = connect(connectSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress));
	if (iResult == SOCKET_ERROR)
	{
		printf("Unable to connect to server.\n");
		closesocket(connectSocket);
		WSACleanup();
		return 1;
	}
#pragma endregion

	char unos = '0';
	while (true)
	{
		memset(&unos, 0, sizeof(unos));
		char* tempBuffer = NULL;
		int size = 0;
		int res = 0;
		//Poziv metode koja ispisuje interakciju sa korisnikom
		menu();

		scanf_s("%c", &unos);
		getchar();
		//ako se unese 4 cisto da se ne ulazi
		if (unos == '4')
			break;

		switch (unos)
		{
		case '1':
			printf("Unesite dimenziju matrice (maksimalno 99):\n");
			scanf_s("%d", &size);
			getchar();
			if (size - 100 < 0)
			{
				tempBuffer = input_matrix(size);
				printf("\nUNETO: %s", tempBuffer);
				break;
			}
			else
			{
				printf("\nDIMENZIJA MORA BITI MANjA OD 100!");
				continue;
			}
			break;
		case '2':
			printf("Unesite dimenziju matrice (maksimalno 99):\n");
			scanf_s("%d", &size);
			getchar();
			if (size - 100 < 0)
			{
				tempBuffer = generate_random_matrix(size);
				printf("\nUNETO: %s", tempBuffer);
				break;
			}
			else
			{
				printf("\nDIMENZIJA MORA BITI MANjA OD 100!");
				continue;
			}
			break;
		case '3':

			res = run_stres_test(connectSocket);
			continue;
			break;
		case '5':
			close_Server(connectSocket);
			break;
		default:
			printf("\nPogresan unos!");
			continue;
			break;
		}
		unos = '0';
		if (tempBuffer != NULL)
			memcpy(dataBuffer, tempBuffer, BUFFER_SIZE);

		free(tempBuffer);
		tempBuffer = NULL;
		//Saljemo serveru podatke
		iResult = send(connectSocket, (char*)dataBuffer, strlen(dataBuffer), 0);

		// Check result of send function
		if (iResult == SOCKET_ERROR)
		{
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(connectSocket);
			WSACleanup();
			return 1;
		}

		printf("Message successfully sent. Total bytes: %ld\n", iResult);

		memset(dataBuffer, 0, BUFFER_SIZE);

		iResult = recv(connectSocket, (char*)dataBuffer, BUFFER_SIZE, 0);
		if (iResult == SOCKET_ERROR)
		{
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(connectSocket);
			WSACleanup();
			return 1;
		}

		dataBuffer[iResult] = '\0';
		printf("\n\nDET = %s", dataBuffer);


	}

	// Shutdown the connection since we're done
	iResult = shutdown(connectSocket, SD_BOTH);

	// Check if connection is succesfully shut down.
	if (iResult == SOCKET_ERROR)
	{
		printf("Shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(connectSocket);
		WSACleanup();
		return 1;
	}


	// Close connected socket
	closesocket(connectSocket);

	delete[] dataBuffer;

	// Deinitialize WSA library
	WSACleanup();
	_getch();
	return 0;
}