
#include "pch.h"
#include"QueueHeader.h"

void initQueue(Queue** head) {
	*head = NULL;
}
#pragma region EnQueue
void EnQueue(Queue** head, Server data) {
	Queue* newElement = (Queue*)malloc(sizeof(Queue));

	newElement->data = data.data;
	newElement->socket = data.socket;
	newElement->next = NULL;

	if (*head == NULL) {
		*head = newElement;
		return;
	}


	Queue* temp = *head;
	while (temp->next != NULL) {
		temp = temp->next;
	}

	temp->next = newElement;

}
#pragma endregion

#pragma region DeQueue
Queue* DeQueue(Queue** head) {

	if (*head == NULL)
		return NULL;

	Queue* data = (Queue*)malloc(sizeof(Queue));

	data->data = (*head)->data;
	data->socket = (*head)->socket;
	data->next = NULL;
	struct Queue* tmp;

	/*Linked list does not exist or the list is empty*/
	if (head == NULL || *head == NULL)
		return NULL;

	/*Storing the head to a temporary variable*/
	tmp = *head;

	/*Moving head to the next node*/
	*head = (*head)->next;

	/*Deleting the first node*/
	free(tmp);


	return data;
}
#pragma endregion





