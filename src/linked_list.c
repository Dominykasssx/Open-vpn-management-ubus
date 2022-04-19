#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

void addToList(struct client **list, struct client *client){
    struct client* temp = *list;
    if (temp == NULL) {
        *list = client;
        return; 
    }

    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = client;
}

struct client* createNodeClient(char *name, char *realAddress, char *bytesSent, char *bytesReceived, char *connectedSince){

    struct client *client = NULL;
    client = (struct client*) malloc(sizeof(struct client));
    if (client == NULL) {
        return NULL;
    }
    strcpy(client->name, name);
    strcpy(client->realAddress, realAddress);
    strcpy(client->bytesSent, bytesSent);
    strcpy(client->bytesReceived, bytesReceived);
    strcpy(client->connectedSince, connectedSince);
    client->next = NULL;
    return client;
}

void deleteList(struct client *list) {
	struct client *to_delete = list;
    while (list != NULL) 
	{
        list = (list)->next;
        free(to_delete);
        to_delete = list;
    }
}
