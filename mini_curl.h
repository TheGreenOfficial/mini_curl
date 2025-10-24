#ifndef MINI_CURL_H
#define MINI_CURL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

static char *mini_curl(const char *hostname) {
    int port = 80;
    struct hostent *host;
    struct sockaddr_in addr;
    int sock;
    char request[512];
    char buffer[4096];
    int n;

    host = gethostbyname(hostname);
    if (!host) return NULL;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return NULL;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr = *((struct in_addr *)host->h_addr);

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) { close(sock); return NULL; }

    snprintf(request, sizeof(request),
             "GET / HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Connection: close\r\n\r\n",
             hostname);

    send(sock, request, strlen(request), 0);

    size_t response_size = 8192;
    char *response = malloc(response_size);
    if (!response) { close(sock); return NULL; }
    response[0] = '\0';
    size_t len = 0;

    while ((n = recv(sock, buffer, sizeof(buffer)-1, 0)) > 0) {
        buffer[n] = '\0';
        if (len + n + 1 > response_size) {
            response_size *= 2;
            char *tmp = realloc(response, response_size);
            if (!tmp) { free(response); close(sock); return NULL; }
            response = tmp;
        }
        strcpy(response + len, buffer);
        len += n;
    }

    close(sock);
    return response;
}

#endif

