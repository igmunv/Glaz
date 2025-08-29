#define BUFFER_LEN 1024
#define MESSAGE_LEN 1024

#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

void http_get(char* address){
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(80);

    inet_pton(AF_INET, address, &server_address.sin_addr);
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0 ) {
        printf("ERROR: Socket\n");
        return -1;
    }

    int conn = connect(sock, (struct sockaddr*)&server_address, sizeof(server_address));
    if (conn < 0 ) {
        printf("ERROR: Connect\n");
        return -1;
    }

    char buffer[BUFFER_LEN];

    char method_line[] = "GET";
    char version_line[] = "HTTP/1.1\r\n";
    char host_line[] = "Host: \r\n";
    char user_agent_line[] = "User-Agent: \r\n";
    char accept_line[] = "Accept: \r\n";
    char connection_line[] = "Connection: \r\n";

    char* request = malloc(MESSAGE_LEN);


    char line1[] = "GET /secret HTTP/1.1\r\nHost: ";
    char line2[] = "\r\nUser-Agent: Glaz/1.0\r\nAccept: text/html\r\nConnection: keep-alive\r\n";
    sprintf(message, "%s%s%s", line1, address, line2);

    send(sock, message, MESSAGE_LEN, 0);
    recv(sock, &buffer, BUFFER_LEN, 0);

    free(message);
    printf("Get:\n%s", buffer);

    close(sock);
    return 0;
}
