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


void printClients(struct client *clients){
	int i = 0;
    struct client *temp = clients;
    
   while (temp != NULL) {
    printf("\n%s | %s | %s | %s | %s | %s | %s \n", temp->name, temp->virtualAddress, temp->realAddress, temp->last_reference,  temp->bytesSent, temp->bytesReceived, temp->connectedSince);
    temp = temp->next;
   }
}