
#include "pch.h"
#include"QueueHeader.h"

void initQueue(Queue** head) {
	*head = NULL;
}
void EnQueue(Queue** head, Server data) {
	Queue* newElement = (Queue*)malloc(sizeof(Queue));

	newElement->data = data.data;
	newElement->socket = data.socket;
	newElement->next = NULL;

	if (*head == NULL) {
		*head = newElement;
		return;
	}

	Queue* helper = *head;
	while (helper->next != NULL) {
		helper = helper->next;
	}

	helper->next = newElement;

}
Queue* DeQueue(Queue** head) {

	if (*head == NULL)
		return NULL;

	Queue* helper = *head;
	Queue* data = (Queue*)malloc(sizeof(Queue));

	data->data = helper->data;
	data->socket = helper->socket;
	data->next = NULL;

	/*if (helper->next->next==nullptr) {
		(*head)->next = helper->next;
	}
	else {
		(*head)->next = helper->next->next;

	}*/
	
	free(helper);

	return data;
}
int NumberOfElements(Queue* head) {
	if (head == NULL)
		return 0;

	int counter = 1;
	while (head->next != NULL) {
		head = head->next;
		counter++;
	}
	return counter;
}