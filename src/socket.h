#define SIZE 2000

int socketConnect(int port, int* oSocket);
int sendCommand(int sock, char *command, char *outBuf);