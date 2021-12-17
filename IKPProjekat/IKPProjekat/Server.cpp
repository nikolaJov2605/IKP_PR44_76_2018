#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include "../Common/Queue.cpp"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#pragma pack(1)

#define SERVER_PORT 27016
#define BUFFER_SIZE 256
#define MAX_CLIENTS 5

HANDLE hSemaphore;

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	while (true)
	{
		/* Cekaj na signal da je pritisnut taster. */
		WaitForSingleObject(hSemaphore, INFINITE);
		/* Povecaj brojac i ispisi vrednost. */
		//Zovemo bolida
	}
}


// TCP server that use non-blocking sockets
int main()
{
	//Role mora jedna * za matricu jer ako su 2 onda se on pogubi imamo ovako jednu *
	//iteriramo po elementima skontacemo kasnije
	Queue* head = (Queue*)malloc(sizeof(Queue));
	initQueue(&head);
	//Server data;
	//float matrix[3][3] = {1,2,3,4,5,6,7,8,9 };
	//int da = 145;
	//Client cl;

	//cl.data = (float*)matrix;
	//cl.size = 3;
	///*for (size_t i = 1; i <= 9; i++)
	//{
	//		printf("%f ", (float)cl.data[i-1]);
	//	if(i%3==0 )
	//		printf("\n");

	//}*/
	//data.data = cl;

	//for (size_t i = 1; i <= 9; i++)
	//{
	//	printf("%f ", (float)data.data.data[i - 1]);
	//	if (i % 3 == 0)
	//		printf("\n");

	//}

	//printf("Hello \n");
	//for (int i = 0; i < 3; i++) {
	//	EnQueue(&head, data);
	//}
	//printf("Ovde");
	//printf("Queue count %d \n", NumberOfElements(head));
	//kasnije ovo obrisati jer je bilo samo za proveru :D
	// Socket used for listening for new clients 

	/* Inicijalizacija sistema. */
	DWORD threadID;
	HANDLE hThread;
	hSemaphore = CreateSemaphore(0, 0, 10, NULL);
	hThread = CreateThread(NULL, 0, &ThreadProc, NULL, 0,
		&threadID);
	

	



	SOCKET listenSocket = INVALID_SOCKET;

	// Sockets used for communication with client
	SOCKET clientSockets[MAX_CLIENTS];
	short lastIndex = 0;

	// Variable used to store function return value
	int iResult;

	// Buffer used for storing incoming data
	char dataBuffer[BUFFER_SIZE];

	// WSADATA data structure that is to receive details of the Windows Sockets implementation
	WSADATA wsaData;

	// Initialize windows sockets library for this process
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartup failed with error: %d\n", WSAGetLastError());
		return 1;
	}

	// Initialize serverAddress structure used by bind
	sockaddr_in serverAddress;
	memset((char*)&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;				// IPv4 address family
	serverAddress.sin_addr.s_addr = INADDR_ANY;		// Use all available addresses
	serverAddress.sin_port = htons(SERVER_PORT);	// Use specific port

	//initialise all client_socket[] to 0 so not checked
	memset(clientSockets, 0, MAX_CLIENTS * sizeof(SOCKET));

	// Create a SOCKET for connecting to server
	listenSocket = socket(AF_INET,      // IPv4 address family
		SOCK_STREAM,  // Stream socket
		IPPROTO_TCP); // TCP protocol

	// Check if socket is successfully created
	if (listenSocket == INVALID_SOCKET)
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket - bind port number and local address to socket
	iResult = bind(listenSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

	// Check if socket is successfully binded to address and port from sockaddr_in structure
	if (iResult == SOCKET_ERROR)
	{
		printf("bind failed with error: %d\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	//// All connections are by default accepted by protocol stek if socket is in listening mode.
	//// With SO_CONDITIONAL_ACCEPT parameter set to true, connections will not be accepted by default
	bool bOptVal = true;
	int bOptLen = sizeof(bool);
	iResult = setsockopt(listenSocket, SOL_SOCKET, SO_CONDITIONAL_ACCEPT, (char*)&bOptVal, bOptLen);
	if (iResult == SOCKET_ERROR) {
		printf("setsockopt for SO_CONDITIONAL_ACCEPT failed with error: %u\n", WSAGetLastError());
	}

	unsigned long  mode = 1;
	if (ioctlsocket(listenSocket, FIONBIO, &mode) != 0)
		printf("ioctlsocket failed with error.");

	// Set listenSocket in listening mode
	iResult = listen(listenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR)
	{
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	printf("Server socket is set to listening mode. Waiting for new connection requests.\n");

	// set of socket descriptors
	fd_set readfds;

	// timeout for select function
	timeval timeVal;
	timeVal.tv_sec = 5;
	timeVal.tv_usec = 0;
	sockaddr_in clientAddr[MAX_CLIENTS];
	memset(clientAddr, 0, MAX_CLIENTS * sizeof(sockaddr_in));
	int clientAddrSize = sizeof(struct sockaddr_in);


	while (true)
	{
		// initialize socket set
		FD_ZERO(&readfds);

		// add server's socket and clients' sockets to set
		if (lastIndex != MAX_CLIENTS)
		{
			FD_SET(listenSocket, &readfds);
		}

		for (int i = 0; i < lastIndex; i++)
		{
			FD_SET(clientSockets[i], &readfds);
		}

		// wait for events on set
		int selectResult = select(0, &readfds, NULL, NULL, &timeVal);

		if (selectResult == SOCKET_ERROR)
		{
			printf("Select failed with error: %d\n", WSAGetLastError());
			closesocket(listenSocket);
			WSACleanup();
			return 1;
		}
		else if (selectResult == 0) // timeout expired
		{
			
			printf("Time Expired \n");
		}
		else if (FD_ISSET(listenSocket, &readfds))
		{
			// Struct for information about connected client
			

			// New connection request is received. Add new socket in array on first free position.
			clientSockets[lastIndex] = accept(listenSocket, (struct sockaddr*)&clientAddr[lastIndex], &clientAddrSize);

			if (clientSockets[lastIndex] == INVALID_SOCKET)
			{
				if (WSAGetLastError() == WSAECONNRESET)
				{
					printf("accept failed, because timeout for client request has expired.\n");
				}
				else
				{
					printf("accept failed with error: %d\n", WSAGetLastError());
				}
			}
			else
			{
				if (ioctlsocket(clientSockets[lastIndex], FIONBIO, &mode) != 0)
				{
					printf("ioctlsocket failed with error.");
					continue;
				}
				lastIndex++;
				printf("New client request accepted (%d). Client address: %s : %d\n", lastIndex, inet_ntoa(clientAddr[lastIndex].sin_addr), ntohs(clientAddr[lastIndex].sin_port));

			}
		}
		else
		{

			// Check if new message is received from connected clients
			for (int i = 0; i < lastIndex; i++)
			{
				// Check if new message is received from client on position "i"
				if (FD_ISSET(clientSockets[i], &readfds))
				{
					iResult = recv(clientSockets[i], dataBuffer, BUFFER_SIZE, 0);

					if (iResult > 0)
					{
						dataBuffer[iResult] = '\0';
						printf("Message received from client (%d):\n", i + 1);


						Client* clientMessage = (Client*)malloc(sizeof(Client));
						//clientMessage->size =(int) ntohs(clientMessage->size);
						printf("Message recieved: %s\n", dataBuffer);

						char separator = ' ';
						char* next;

						char* token = strtok_s(dataBuffer, &separator, &next);

						clientMessage->size = atoi(token);							// od prvog elementa niza preuzima dimenziju matrice

						float* matrix_data = (float*)malloc(clientMessage->size * clientMessage->size);
						int idx = 0;
						while (strcmp(next, "") != 0)								// od ostalih elemenata pravimo niz
						{
							printf("\nTOKEN: %s", next);
							token = strtok_s(next, &separator, &next);
							matrix_data[idx] = atof(token);
							idx++;
						}

						for (int i = 0; i < clientMessage->size * clientMessage->size; i++)
						{
							printf("\n%f", matrix_data[i]);
						}

						
						
						//pustamo semafor da moze da racuna
						//ReleaseSemaphore(hSemaphore, 1, NULL);

						printf("_______________________________  \n");

						//free(matrix_data);
						//free(clientMessage);
					}
					else if (iResult == 0)
					{
						// connection was closed gracefully
						printf("Connection with client (%d) closed.\n", i + 1);
						closesocket(clientSockets[i]);

						// sort array and clean last place
						for (int j = i; j < lastIndex - 1; j++)
						{
							clientSockets[j] = clientSockets[j + 1];
						}
						clientSockets[lastIndex - 1] = 0;

						for (int j = i; j < lastIndex - 1; j++)
						{
							clientAddr[j] = clientAddr[j + 1];
						}
						memset(&clientAddr[lastIndex - 1], 0, sizeof(sockaddr_in));

						lastIndex--;
					}
					else
					{
						// there was an error during recv
						printf("recv failed with error: %d\n", WSAGetLastError());
						closesocket(clientSockets[i]);

						// sort array and clean last place
						for (int j = i; j < lastIndex - 1; j++)
						{
							clientSockets[j] = clientSockets[j + 1];
						}
						clientSockets[lastIndex - 1] = 0;


						for (int j = i; j < lastIndex - 1; j++)
						{
							clientAddr[j] = clientAddr[j + 1];
						}
						memset(&clientAddr[lastIndex - 1], 0, sizeof(sockaddr_in));


						lastIndex--;
					}
				}
			}
		}
	}

	//Close listen and accepted sockets
	closesocket(listenSocket);

	// Deinitialize WSA library
	WSACleanup();

	return 0;
}
