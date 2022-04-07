#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>
#include "socket.h"

int socketConnect(int port, int* oSocket)
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[SIZE] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
       
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    *oSocket = sock;
    return 0;
}

int sendCommand(int sock, char *command, char *outBuf){
    char buffer[SIZE];
    int valread;
    int rc = send(sock , command , strlen(command) , 0 );
    sleep(0.5);
    printf("Command %s sent\n", command);
    valread = read( sock , buffer, SIZE);
    snprintf(outBuf, SIZE, "%s", buffer);
    return rc;
}