#define BUFFER_LEN 1024
#define MESSAGE_LEN 1024

#include <sys/socket.h>
#include <arpa/inet.h>

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

    char message[MESSAGE_LEN];
    char buffer[BUFFER_LEN];

    // сервер нихуя не поймёт
    message[0] = 'x';
    message[1] = 'u';
    message[2] = 'i';

    send(sock, message, MESSAGE_LEN, 0);
    recv(sock, &buffer, BUFFER_LEN, 0);

    printf("Get: %s", buffer);

    close(sock);
    return 0;
}
