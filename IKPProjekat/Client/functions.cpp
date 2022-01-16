#include "functions.h"
#include "constants.h"


void menu() {
	printf("\n 1 - Izracunaj matricu\n");
	printf(" 2 - Generisi random matricu\n");
	printf(" 3 - Pokreni stres test\n");
	printf(" 4 - Izadjete iz porgrama\n");
	printf(" 5 - Test ciscenja resursa\n");
	printf(" -> ");

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
	srand(time(0));
	char stringSize[3];
	char dataBuffer[BUFFER_SIZE];
	char* retBuffer = (char*)malloc(BUFFER_SIZE);
	_itoa_s(size, stringSize, 10);
	char raz[2] = " ";
	int* niz = new int[pow(size, 2)];

	strncat_s(stringSize, raz, strlen(raz));
	dataBuffer[0] = 0;
	retBuffer[0] = 0;
	strncat_s(dataBuffer, stringSize, strlen(stringSize));
	char converted[10];
	int sign = 0;
	printf("\n");
	for (int i = 0; i < pow(size, 2); i++)
	{
		sign = rand() % 2;
		niz[i] = rand() % 40 + 1;
		if (sign)
			niz[i] *= -1;
		printf("%d ", niz[i]);
		_itoa_s(niz[i], converted, 10);

		strncat_s(converted, raz, strlen(raz));
		strncat_s(dataBuffer, converted, strlen(converted));
		memset(converted, 0, 4);
	}

	memcpy(retBuffer, dataBuffer, BUFFER_SIZE);

	return retBuffer;
}


int run_stres_test(SOCKET connectSocket)
{
	//kreiram novi soket jer  cu da ga prebacim u non-blocking rezim


	// Variable used to store function return value
	int iResult;

	// Buffer we will use to store message
	char* dataBuffer = new char[BUFFER_SIZE];
	bool valid = false;



	char* sendingBuffer;
	int result = 0;
	// petlja koja salje zahteve serveru
	//uspavljujemo nit jer seed ne moze da isprati 
	int times = 5;
	for (int i = 0; i < times; i++)
	{
		int size = rand()% 7 + 2;
		Sleep(200);
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
	}

	//non-blocking mode 
	unsigned long  mode = 1;
	if (ioctlsocket(connectSocket, FIONBIO, &mode) != 0)
		printf("ioctlsocket failed with error.");
	fd_set readfds;

	// timeout for select function
	timeval timeVal;
	timeVal.tv_sec = 0;
	timeVal.tv_usec = 0;
	int clientAddrSize = sizeof(struct sockaddr_in);

	int counter = 0;
	// vrtim se stalno i cekam da primim poruku
	//uspavljujemo nit samo da moze lepo da se ispise na konzoli
	char** recBuffer = new char* [times];
	for (size_t i = 0; i < times; i++)
	{
		recBuffer[i] = new char[20];
	}
	while (true) {
		FD_ZERO(&readfds);
		FD_SET(connectSocket, &readfds);

		int iResult = 0;
		iResult = select(0, &readfds, NULL, NULL, &timeVal);

		if (iResult == SOCKET_ERROR)
		{
			printf("Select failed with error: %d\n", WSAGetLastError());
			closesocket(iResult);
			WSACleanup();
			return 1;
		}
		else if (iResult == 0) // timeout expired
		{
			continue;
		}
		else {
			iResult = recv(connectSocket, recBuffer[counter], BUFFER_SIZE, 0);

			if (iResult > 0)
			{
				
				recBuffer[counter][iResult] = '\0';

				printf("\n Result %s\n", recBuffer[counter]);
				memset(dataBuffer, 0, BUFFER_SIZE);

				if (++counter == times)
					break;
			}
		}
	}
	for (size_t i = 0; i < times; i++)
	{
		delete[] recBuffer[i];
	}
	delete[] recBuffer;
	mode = 0;
	if (ioctlsocket(connectSocket, FIONBIO, &mode) != 0)
		printf("ioctlsocket failed with error.");
	return result;
}

void close_Server(SOCKET socket) {
	char sendingBuffer[5] = "Exit";

	int iResult = send(socket, (char*)sendingBuffer, strlen(sendingBuffer), 0);

	if (iResult == SOCKET_ERROR)
	{
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(socket);
		WSACleanup();
		return;
	}
}