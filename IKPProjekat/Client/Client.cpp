#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "../Common/Queue.cpp"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#pragma pack(1)

#define SERVER_IP_ADDRESS "127.0.0.1"
#define SERVER_PORT 27016
#define BUFFER_SIZE 256


char* input_matrix(int size);
char* generate_random_matrix(int size);
/*int run_stres_test(SOCKET connectSocket);*/

// TCP client that use non-blocking sockets
int main()
{
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


	Sleep(1000);


	// Connect to server specified in serverAddress and socket connectSocket
	iResult = connect(connectSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress));
	if (iResult == SOCKET_ERROR)
	{
		printf("Unable to connect to server.\n");
		closesocket(connectSocket);
		WSACleanup();
		return 1;
	}



	while (true)
	{
		char* tempBuffer = NULL;
		int size = 0;
		int res = 0;
		// pripremamo buffer za slanje za upis podataka;
		//dataBuffer[0] = 0;
		//memcpy(dataBuffer, 0, BUFFER_SIZE);
		// Unos potrebnih podataka koji ce se poslati serveru
		char unos;

		printf("\n1 - Izracunaj matricu\n");
		printf("2 - Generisi random matricu\n");
		printf("3 - Pokreni stres test\n");
		printf(" -> ");
		scanf_s("%c", &unos);

		switch (unos)
		{
		case '1':
			printf("Unesite dimenziju matrice (maksimalno 99):\n");
			scanf_s("%d", &size);
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
			//res = run_stres_test(connectSocket);
			break;
		default:
			printf("\nPogresan unos!");
			break;
		}

		if (tempBuffer != NULL)
			memcpy(dataBuffer, tempBuffer, BUFFER_SIZE);

		free(tempBuffer);

		//prosledjujemo adresu promenljive student u memoriji, jer se na toj adresi nalaze podaci koje saljemo
		//kao i velicinu te strukture (jer je to duzina poruke u bajtima)
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


		printf("\nPress 'x' to exit or any other key to continue: ");
		if (_getch() == 'x')
			break;
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

	Sleep(1000);

	// Close connected socket
	closesocket(connectSocket);

	delete[] dataBuffer;

	// Deinitialize WSA library
	WSACleanup();

	return 0;
}


char* input_matrix(int size)
{
	bool valid = false;
	char raz = ' ';
	char stringSize[3];
	char dataBuffer[BUFFER_SIZE];
	char* retBuffer = (char*)malloc(BUFFER_SIZE);
	_itoa_s(size, stringSize, 10);

	strncat_s(stringSize, &raz, 1);
	dataBuffer[0] = 0;
	retBuffer[0] = 0;
	strncat_s(dataBuffer, stringSize, strlen(stringSize));

	printf("%s", dataBuffer);


	for (size_t i = 0; i < size * size; i++)
	{
		int brojac = 0;
		char number[100];												// string u koji ce se upisivati broj
		printf("\n%d) ", i + 1);
		scanf_s("%s", number, 100);
		for (int j = 0; number[j] != 0; j++)							// provera da li je broj validan
		{
			valid = true;
			if (number[j] < 48 || number[j] > 57)
			{
				valid = false;
				printf("\nPlease insert a valid number!\n");
				i--;
				break;
			}
			brojac++;
		}
		if (valid)
		{
			strncat_s(number, &raz, 1);
			strncat_s(dataBuffer, number, strlen(number));
		}
	}

	printf("\nBUFFER: %s", dataBuffer);

	memcpy(retBuffer, dataBuffer, BUFFER_SIZE);

	return retBuffer;
}


char* generate_random_matrix(int size)
{
	char stringSize[3];
	char dataBuffer[BUFFER_SIZE];
	char* retBuffer = (char*)malloc(BUFFER_SIZE);
	_itoa_s(size, stringSize, 10);
	char raz = ' ';
	int* niz = new int[pow(size, 2)];

	strncat_s(stringSize, &raz, 1);
	dataBuffer[0] = 0;
	retBuffer[0] = 0;
	strncat_s(dataBuffer, stringSize, strlen(stringSize));
	char converted[10];

	srand(time(0));
	printf("\n");
	for (int i = 0; i < pow(size, 2); i++)
	{
		niz[i] = rand() % 200 + 1;
		printf("%d ", niz[i]);
		_itoa_s(niz[i], converted, 10);

		strncat_s(converted, &raz, 1);
		strncat_s(dataBuffer, converted, strlen(converted));
		memset(converted, 0, 4);
	}

	memcpy(retBuffer, dataBuffer, BUFFER_SIZE);

	return retBuffer;
}



/*int run_stres_test(SOCKET connectSocket)
{
	srand(time(0));
	char* sendingBuffer;
	int iResult;
	int result = 0;

	for (int i = 0; i < 10; i++)
	{
		int size = rand() % 5 + 2;
		sendingBuffer = generate_random_matrix(size);

		iResult = send(connectSocket, (char*)sendingBuffer, strlen(sendingBuffer), 0);

		if (iResult == SOCKET_ERROR)
		{
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(connectSocket);
			WSACleanup();
			return 1;
		}

		printf("Message successfully sent. Total bytes: %ld\n", iResult);

		free(sendingBuffer);
		memset(sendingBuffer, 0, 2048);
	}

	for (int i = 0; i < 10; i++)
	{
		memset(sendingBuffer, 0, 2048);
		iResult = recv(connectSocket, (char*)sendingBuffer, BUFFER_SIZE, 0);
		if (iResult == SOCKET_ERROR)
		{
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(connectSocket);
			WSACleanup();
			return 1;
		}

		sendingBuffer[iResult] = '\0';
		printf("\n\nDET = %s", sendingBuffer);
	}
}*/