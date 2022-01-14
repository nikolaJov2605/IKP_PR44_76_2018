#pragma once
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>

typedef struct ServerPacket_st {
	char* data;
	SOCKET socket;
}Server;
