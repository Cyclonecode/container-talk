#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>

#ifndef LOCAL_PORT
#define LOCAL_PORT 2048
#endif
#ifndef REMOTE_NAME
#define REMOTE_NAME "foo"
#endif
#ifndef REMOTE_PORT
#define REMOTE_PORT 1024
#endif

void* serverThread(void* args) {
    printf("creating server thread\n");
    struct sockaddr_in addr;
    int* ret = args;
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1) {
        perror("Failed to create socket.");
        *ret = -1;
        return ret;
    }
    printf("Successfully created server socket.\n");
    int enable = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) != 0) {
        perror("Failed to reuse address.");
        close(sock);
        *ret = -1;
        return ret;
    }
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) != 0) {
        perror("Failed to reuse port.");
        close(sock);
        *ret = -1;
        return ret;
    }
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(LOCAL_PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
        perror("Failed to bind to address x.");
        close(sock);
        *ret = -1;
        return ret;
    }
    printf("Successfully bind to server address.\n");
    if (listen(sock, 0) != 0) {
        perror("Failed to listen on socket.");
        close(sock);
        *ret = -1;
        return ret;
    }
    printf("Successfully started listen on socket %s:%d.\n", inet_ntoa(*(struct in_addr*)&addr.sin_addr.s_addr), LOCAL_PORT);
    fflush(stdout);
    int counter = 0;
    char data[1024];
    struct sockaddr_in caddr;
    socklen_t length = sizeof(caddr);
    fflush(stdout);
    while (1) {
        printf("Start waiting for clients.\n");
        fflush(stdout);
        int csock = accept(sock, (struct sockaddr*)&caddr, &length);
        int sent = 0;
        int read = 0;
        do {
            read = recv(csock, data, 1024, 0);
            if (strncmp(data, "ping", 4) == 0) {
                printf("Server: Received ping request from %s:%d\n", inet_ntoa(*(struct in_addr*)&caddr.sin_addr.s_addr), ntohs(caddr.sin_port));
                printf("Server: Sending pong to client %s:%d\n", inet_ntoa(*(struct in_addr*)&caddr.sin_addr.s_addr), ntohs(caddr.sin_port));
                usleep(WAIT);
                sent = send(csock, "pong", 4, 0);
            }
            // usleep(500);
            fflush(stdout);
        } while(read == 4 && sent == 4);
        close(csock);
        usleep(WAIT);
    }
    printf("after waiting for clients\n");
    fflush(stdout);
    close(sock);
    return ret;
}

unsigned long getInetAddr(const char* nameOrIp) {
    struct hostent* phe;
    if ((phe = gethostbyname(nameOrIp))) {
        return *((unsigned long*) phe->h_addr);
    } else {
        return inet_addr(nameOrIp);
    }
}

void* clientThread(void* args) {
    printf("creating client thread\n");
    struct sockaddr_in caddr, saddr;
    struct hostent* phe;
    int* ret = args;
    int csock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (csock == -1) {
        perror("Failed to create client socket.");
        *ret = -1;
        return ret;
    }
    printf("Successfully created client socket.\n");
    memset(&caddr, 0, sizeof(caddr));
    caddr.sin_family = AF_INET;
    caddr.sin_port = htons(0);
    caddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(csock, (struct sockaddr*)&caddr, sizeof(caddr)) != 0) {
        printf("Failed to bind to client address.");
        *ret = -1;
        return ret;
    }
    usleep(10000);
    printf("Successfully bind to client address.\n");
    fflush(stdout);
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(REMOTE_PORT);
    saddr.sin_addr.s_addr = getInetAddr(REMOTE_NAME);
    if (connect(csock, (struct sockaddr*)&saddr, sizeof(saddr)) != 0) {
        printf("Failed connecting to server: %s:%d.", inet_ntoa(*(struct in_addr*)&saddr.sin_addr.s_addr), REMOTE_PORT);
        fflush(stdout);
        *ret = -1;
        return ret;
    }
    printf("Successfully connected to server.\n");
    char data[1024];
    int sent = 0;
    int read = 0;
    do {
        printf("Client: Sending ping to %s:%d\n", inet_ntoa(*(struct in_addr*)&saddr.sin_addr.s_addr), ntohs(saddr.sin_port));
        sent = send(csock, "ping", 4, 0);
        usleep(WAIT);
        read = recv(csock, data, 1024, 0);
        printf("Client: Received %s from %s:%d\n", data, inet_ntoa(*(struct in_addr*)&saddr.sin_addr.s_addr), ntohs(saddr.sin_port));
        fflush(stdout);
        usleep(WAIT);
    } while(sent == 4 && read == 4);
    close(csock);

    return ret;
}

int main(int argc, char** argv) {
    printf("%s %d %d\n", REMOTE_NAME, REMOTE_PORT, LOCAL_PORT);
    pthread_t threads[2];
    int error;
    int ret[2];
    error = pthread_create(&threads[0], NULL, &serverThread, &ret[0]);
    if (error != 0) {
        printf("Failed to create server thread\n");
        return -1;
    }
    printf("waiting 10000ms to create client\n");
    usleep(10000);
    printf("creating client\n");
    error = pthread_create(&threads[1], NULL, &clientThread, &ret[1]);
    if (error != 0) {
        // pthread_close();
        printf("Failed to create client thread\n");
        return -1;
    }
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    return 0;
}
