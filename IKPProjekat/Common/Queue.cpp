#include"QueueHeader.h"

void initQueue(Queue** head) {
	*head = NULL;
}
void EnQueue(Queue** head, Server data) {
	Queue* newElement = (Queue*)malloc(sizeof(Queue));

	newElement->data =data;
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
	Queue* data = NULL;
	data->data = helper->data;

	(*head)->next = helper->next->next;
	
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