// Jakub Żmuda
#include "../common.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>

void sendUptimeToClient(int socket);

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

    handleRequest(serverSocketId, sendUptimeToClient, 0);
}

void sendUptimeToClient (int socket) {
    int bufferSize = 128 + 1;
    char buffer[bufferSize];

    char *cmd = "uptime";
    FILE *fp;

    if ((fp = popen(cmd, "r")) == NULL) {
        printf("Error opening pipe!\n");
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, bufferSize, fp) != NULL) {
    }

    if(pclose(fp))  {
        printf("Command not found or exited with error status\n");
        exit(EXIT_FAILURE);
    }

    printf("sending: %s\n", buffer);

    write(socket, buffer, strlen(buffer));
}