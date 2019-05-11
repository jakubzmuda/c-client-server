// Jakub Żmuda
#include "../../common.h"

void handleResponse(int socket);

int main(int argc, char *argv[]) {
     if(argc != 3) {
        printf("usage: address port \n");
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[2]);
    int socketId = createUdpSocket();
    bindPort(socketId, port);

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);

    struct sockaddr_in clientaddr;
    memset(&clientaddr, 0, sizeof(clientaddr));
    socklen_t clientaddrLength = sizeof(clientaddr);
    int bytesReceived = recvfrom(socketId, NULL, 0,
                     MSG_WAITALL, (struct sockaddr *) &clientaddr,
                     &clientaddrLength);
    if(bytesReceived < 0) {
        printf("failed to receive data");
        exit(EXIT_FAILURE);
    }

    long s = 2208988800L;
    uint32_t buffer = htonl(time(NULL) + s);
    int bytesSent = sendto(socketId, &buffer, 32,
           MSG_CONFIRM, (const struct sockaddr *) &clientaddr, sizeof(clientaddr));
    if(bytesSent < 0) {
        printf("failed to send data\n");
        exit(EXIT_FAILURE);
    }

    printf("message sent.\n");
    return 0;
}