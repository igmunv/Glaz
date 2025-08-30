#define BUFFER_LEN 1024
#define MESSAGE_LEN 1024

#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct header_struct{
    unsigned char* head;
    short size;
} header_t;

int get_string_len(char* string){
    int len = 0;
    while (1){
        if (string[len] != '\0') len+=1;
        else return len;
    }
}

char is_end_head(char a, char b, char c, char d){
    if (a == '\r' && b == '\n' && c == '\r' && d == '\n') return 1;
    else return 0;
}

char is_end_str(char a, char b){
    if (a == '\r' && b == '\n') return 1;
    else return 0;
}

unsigned short get_head(unsigned char* response, int response_size, unsigned char* buffer){

    for (unsigned short i = 0; i < response_size-4; i++){

        buffer[i] = response[i];

        if (response[i] == '\0') return -1;
        if (
            response[i] == '\r' &&
            response[i + 1] == '\n' &&
            response[i + 2] == '\r' &&
            response[i + 3] == '\n'
        ) {
            buffer[i] = '\n';
            return i;
        }
    }
    return -1;
}

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

    char path[] = "/secret/";
    char accept_language[] = "en-US,en;q=0.9";
    char user_agent[] = "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/136.0.0.0 Safari/537.36";
    char accept[] = "*/*";
    char accept_encoding[] = "identity";
    char connection[] = "close";

    char method_line[] = "GET";
    char version_line[] = "HTTP/1.1";
    char host_line[] = "Host:";
    char accept_language_line[] = "Accept-Language:";
    char user_agent_line[] = "User-Agent:";
    char accept_line[] = "Accept:";
    char accept_encoding_line[] = "Accept-Encoding:";
    char connection_line[] = "Connection:";

    char* request = malloc(MESSAGE_LEN);

    sprintf(request, "%s %s %s\r\n%s %s\r\n%s %s\r\n%s %s\r\n%s %s\r\n%s %s\r\n%s %s\r\n\r\n", method_line, path, version_line, host_line, address, accept_language_line, accept_language, user_agent_line, user_agent, accept_line, accept, accept_encoding_line, accept_encoding, connection_line, connection);

    int msg_len = get_string_len(request);

    printf("REQUEST (size: %d):\n\n", msg_len);
    printf("%s\n", request);

    send(sock, request, msg_len, 0);

    int recv_bytes ;
    int response_size = 0;
    while ((recv_bytes = recv(sock, &buffer, BUFFER_LEN-1, 0)) > 0){
        buffer[recv_bytes] = '\0';
        response_size += recv_bytes;
        //printf("\n%d\n\n", recv_bytes);

        printf("%s", buffer);
    }

    printf("\nrecv_bytes = %d\n", response_size);

    char head[32768];
    unsigned short head_size = 0;
    head_size = get_head(buffer, response_size, &head);

    printf(" -- %s -- \n -- %d --\n", head, head_size);

    free(request);


    close(sock);
    return 0;
}
