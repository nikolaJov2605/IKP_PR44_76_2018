
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
	

	Queue* temp = *head;
	while (temp->next != NULL) {
		temp = temp->next;
	}

	temp->next = newElement;

}
Queue* DeQueue(Queue** head) {

	if (*head == NULL)
		return NULL;

	Queue* helper = *head;
	Queue* data = (Queue*)malloc(sizeof(Queue));

	data->data = helper->data;
	data->socket = helper->socket;
	data->next = NULL;

	
	free(helper);

	return data;
}


/* Create an empty queue */
void initialize() {
    front = back = NULL;
}

/*
Adding elements in Queue
*/
void enqueue(node** head, Server data) {
	node* newElement = (node*)malloc(sizeof(node));

	newElement->data = data.data;
	newElement->socket = data.socket;
	newElement->next = NULL;


    if (back == NULL) {
        front = back = newElement;
    }
    else {
        back->next = newElement;
    }
}

/*
Removes an element from front of the queue
*/
void dequeue() {

	node* data = (node*)malloc(sizeof(node));
	
	
    if (front == NULL) {
        printf("\nQueue is Empty \n");
        return;
    }
    else {
        data = front;
        front = front->next;
        if (front == NULL) {
            back = NULL;
        }
		//free(data);
    }
}
node* frontelement() {
	if ((front != NULL) && (back != NULL))
		return front;
	else
		return 0;
}