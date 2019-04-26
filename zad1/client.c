// Jakub Żmuda
#include "../common.h"

void handleResponse(int socket);

int main(int argc, char *argv[]) {

    if(argc != 2) {
        printf("enter port \n");
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);
    int clientSocketId = createTcpSocket();
    createTcpConnection(clientSocketId, "127.0.0.1", port);
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