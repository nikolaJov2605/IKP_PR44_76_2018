#pragma once

typedef struct Queue {
	char* data;
	SOCKET socket;
	struct Queue* next;
}Queue;