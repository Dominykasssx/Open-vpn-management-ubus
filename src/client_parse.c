#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>
#include "linked_list.h"

int parseClient(char *buffer, struct client **list)
{
  struct client *clients = NULL;
  char *name;
  char *realAddress;
  char *virtualAddress;
  char *bytesSent;
  char *bytesReceived;
  char *connectedSince;
  char *last_reference;
  char *token;
  int count = 0;

  char delimeter[] = "\n\r,";
  token = strtok(buffer, delimeter);

  while (token != NULL && strncmp(token, "Connected Since", strlen("Connected Since")) != 0){
    token = strtok(NULL, delimeter);
  }

  if (strncmp(token, "Connected Since", strlen("Connected Since")) == 0){
    token = strtok(NULL, delimeter);
  }

  while (token != NULL && strncmp(token, "ROUTING TABLE", strlen("ROUTING TABLE")) != 0){
    switch (count){
    case 0:
      name = token;
      count++;
      break;
    case 1:
      realAddress = token;
      count++;
      break;
    case 2:
      bytesSent = token;
      count++;
      break;
    case 3:
      bytesReceived = token;
      count++;
      break;
    case 4:
      connectedSince = token;
      count = 0;
      clients = createNodeClient(name, realAddress, bytesSent, bytesReceived, connectedSince);
      addToList(list, clients);
      break;
    default:
      count = 0;
    }
    token = strtok(NULL, delimeter);
  }

  while (token != NULL && strncmp(token, "Last Ref", strlen("Last Ref")) != 0){

    token = strtok(NULL, delimeter);
  }
  if (strncmp(token, "Last Ref", strlen("Last Ref")) == 0){
    token = strtok(NULL, delimeter);
  }

  struct client *temp = *list;
  count = 0;

  while (temp != NULL){
    while (token != NULL && strncmp(token, "GLOBAL STATS", strlen("GLOBAL STATS")) != 0){
      switch (count){
      case 0:
        strcpy(temp->virtualAddress, token);
        count++;
        break;
      case 1:
        count++;
        break;
      case 2:
        count++;
        break;
      case 3:
        strcpy(temp->last_reference, token);
        count++;
        break;
      }
      if (count == 4){
        token = strtok(NULL, delimeter);
        count = 0;
        break;
      }
      else{
        token = strtok(NULL, delimeter);
      }
    }
    temp = temp->next;
  }
  return 0;
}