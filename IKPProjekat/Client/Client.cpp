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


// TCP client that use non-blocking sockets
int main()
{
	// Socket used to communicate with server
	SOCKET connectSocket = INVALID_SOCKET;

	// Variable used to store function return value
	int iResult;

	// Buffer we will use to store message
	char dataBuffer[BUFFER_SIZE];

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

	//promenljiva tipa studentInfo cija ce se polja popunuti i cela struktira poslati u okviru jedne poruke
	

	while (true)
	{
		// Unos potrebnih podataka koji ce se poslati serveru
		Client data;

		printf("Unesite dim matrice \n");
		scanf_s("%d", &data.size);
		float matrix[3][3];
		float da;
		for (size_t i = 0; i < data.size ;i++)
		{
			for (size_t j = 0; j < data.size; j++)
			{
				scanf_s("%f", &matrix[i][j]);
			}
		}
		data.data = (float*)matrix;
		printf("%d , %d", &matrix, data.data);
		data.size = htons(data.size);


		//obavezna funkcija htons() jer cemo slati podatak tipa short 
		getchar();    //pokupiti enter karakter iz bafera tastature


		// Slanje pripremljene poruke zapisane unutar strukture studentInfo
		//prosledjujemo adresu promenljive student u memoriji, jer se na toj adresi nalaze podaci koje saljemo
		//kao i velicinu te strukture (jer je to duzina poruke u bajtima)
		iResult = send(connectSocket, (char*)&data, (int)sizeof(Client), 0);

		// Check result of send function
		if (iResult == SOCKET_ERROR)
		{
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(connectSocket);
			WSACleanup();
			return 1;
		}

		printf("Message successfully sent. Total bytes: %ld\n", iResult);

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

	// Deinitialize WSA library
	WSACleanup();

	return 0;
}
