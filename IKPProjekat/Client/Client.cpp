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


	char raz = ' ';

	while (true)
	{

		// pripremamo buffer za slanje za upis podataka;
		dataBuffer[0] = 0;

		// Unos potrebnih podataka koji ce se poslati serveru
		int size = 0;
		while (true)// preko do while odraditi
		{
			printf("Unesite dimenziju matrice (maksimalno 99):\n");

			scanf_s("%d", &size);
			if (size - 100 < 0)
				break;
			printf("\nDIMENZIJA MORA BITI MANjA OD 100!");
		}

		

		char stringSize[3];
		_itoa_s(size, stringSize, 10);

		strncat_s(stringSize, &raz, 1);

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
		
		//obavezna funkcija htons() jer cemo slati podatak tipa short 
		getchar();    //pokupiti enter karakter iz bafera tastature


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
