// Jakub Żmuda
#include "../common.h"

void handleResponse(int socket);

int main(int argc, char *argv[]) {

    if(argc != 3) {
        printf("usage: ./client domainAddress port \n");
        exit(EXIT_FAILURE);
    }

    char* address = lookupDomain(argv[1]);

    printf("resolved domain address to %s, now attempting to fetch the data\n", address);

    int port = atoi(argv[2]);
    int clientSocketId = createTcpSocket();
    createTcpConnection(clientSocketId, address, port);
    handleResponse(clientSocketId);
    closeConnection(clientSocketId);
}

void handleResponse (int socket) {
    int in;
    char buffer[128 + 1];

    while ((in = read(socket, buffer, 128)) > 0) {
        buffer[in] = 0;
    }

    printf("server response: %s\n", buffer);
}