struct client
{
    char name[200];
    char realAddress[50];
    char virtualAddress[50];
    char bytesSent[120];
    char bytesReceived[120];
    char connectedSince[25];
    char last_reference[25];
    struct client *next;
};

void addToList(struct client **list, struct client *client);
struct client* createNodeClient(char *name, char *realAddress, char *bytesSent, char *bytesReceived, char *connectedSince);
void printClients(struct client *clients);
void deleteList(struct client *list);