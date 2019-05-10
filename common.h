// Jakub Żmuda
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netdb.h>
#include <netinet/in.h>

#define MAX_BUFFER 128

int createSocket (int type) {
    int socketId;

    socketId = socket(AF_INET, type, 0);

    if (socketId == -1) {
        fprintf(stderr, "error creating a socket %s\n", strerror(errno));
        exit(EXIT_FAILURE);
        return socketId;
    }

    return socketId;
}

int createUdpSocket () {
    return createSocket(SOCK_DGRAM);
}

int createTcpSocket () {
    return createSocket(SOCK_STREAM);
}

int closeConnection (int socket) {
     int status = close(socket);

     if (status == -1) {
        fprintf(stderr, "unable to close connection for socket %d: %s\n", socket, strerror(errno));
        exit(EXIT_FAILURE);
        return status;
     }

     return status;
}

int bindPort (int socket, int port) {
    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);
    memset( &( address.sin_zero ), '\0', 8 );

    int bindStatus = bind(socket, (struct sockaddr *) & address, sizeof(struct sockaddr));

    if (bindStatus == -1) {
        fprintf(stderr, "unable to bind port %d: %s\n", port, strerror(errno));
        exit(EXIT_FAILURE);
        return bindStatus;
    }

    return bindStatus;
}

void handleRequest (int socket, void (*func)(), int use_syslog) {
    int freshSocket, closeStatus;
    struct sockaddr_in address;
    socklen_t address_len = sizeof(struct sockaddr_in);
    struct in_addr clientIp;
    char * ip;
    unsigned short port;

    while ( 1 ) {
        freshSocket = accept(socket, (struct sockaddr *) & address, & address_len);
        clientIp = address.sin_addr;
        ip = inet_ntoa(clientIp);
        port = ntohs(address.sin_port);

        if (freshSocket == -1) {
            fprintf(stderr, "unable to accept client %s:%d: %s\n", ip, port, strerror(errno));
            exit(EXIT_FAILURE);
        }

        func(freshSocket);

        closeStatus = close(freshSocket);

        if (closeStatus == -1) {
            fprintf(stderr, "unable to close connection with client %s:%d: %s\n", ip, port, strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
}

void receiveFromServer (int socket) {
    int in;
    char buffer[MAX_BUFFER + 1];

    while ((in = read(socket, buffer, MAX_BUFFER)) > 0) {
        buffer[in] = 0;
        printf("\n%s", buffer);
    }
}

int createTcpConnection (int socket, const char * ip, int port) {
    int connectStatus;
    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip);
    address.sin_port = htons(port);
    memset( &( address.sin_zero ), '\0', 8 );

    connectStatus = connect(socket, (struct sockaddr *) & address, sizeof(struct sockaddr));

    if (connectStatus == -1) {
        fprintf(stderr, "unable to create connection to socket %d: %s\n", socket, strerror(errno));
        exit(EXIT_FAILURE);
        return connectStatus;
    }

    return connectStatus;
}

int listenForClient (int socket, int backlog) {
    int listenStatus = listen(socket, backlog);

    if (listenStatus == -1) {
        fprintf(stderr, "unable to bind for socket %d with backlog %d: %s\n", socket, backlog, strerror(errno));
        exit(EXIT_FAILURE);
        return listenStatus;
    }

    return listenStatus;
}

char* lookupDomain(char* domain) {
struct addrinfo hints, *res, *res0;
	int error;
	static char host[NI_MAXHOST];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	error = getaddrinfo(domain, NULL, &hints, &res0);
	if (error) {
		fprintf(stderr, "%s\n", gai_strerror(error));
		exit(1);
	}

	for (res = res0; res; res = res->ai_next) {
		error = getnameinfo(res->ai_addr, res->ai_addrlen,
		    host, sizeof host, NULL, 0, NI_NUMERICHOST);

		if (error) {
			fprintf(stderr, "dns lookup error: %s\n", gai_strerror(error));
		}
	}

	freeaddrinfo(res0);

    return host;
}
