// Jakub Żmuda
#include "../../common.h"

void handleResponse(int socket);

int main(int argc, char *argv[]) {

    if(argc != 3) {
        printf("usage: domainName port \n");
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[2]);
    int socketId = createUdpSocket();

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr(lookupDomain(argv[1]));

    int bytesSent = sendto(socketId, NULL, 0,
           MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
    if(bytesSent < 0) {
        printf("failed to send data\n");
        exit(EXIT_FAILURE);
    }

    uint32_t buffer;
    socklen_t servaddrLength = sizeof(servaddr);
    int bytesReceived = recvfrom(socketId, &buffer, 32,
                     MSG_WAITALL, (struct sockaddr *) &servaddr,
                     &servaddrLength);
    if(bytesReceived < 0) {
        printf("failed to receive data");
        exit(EXIT_FAILURE);
    }
    uint32_t seconds = ntohl(buffer);
    long s = 2208988800L;
    const long int secondsConverted = (long int) seconds - s;
    printf("Received timestamp : %s\n", ctime(&secondsConverted));
}