// Jakub Żmuda
#include "../common.h"

void sendHelloToClient(int socket);

int main(int argc, char *argv[]) {

    if(argc != 2) {
        printf("enter the port\n");
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);
    int serverSocketId = createTcpSocket();
    bindPort(serverSocketId, port);
    listenForClient(serverSocketId, 128);

    printf("Spawned server and listening at port %i\n", port);

    handleRequest(serverSocketId, sendHelloToClient, 0);
}

void sendHelloToClient (int socket) {
    char buffer[128 + 1];

    char* message = "hello";
    snprintf(buffer, 128, "%s\n", message);

    write(socket, buffer, strlen(buffer));
}
